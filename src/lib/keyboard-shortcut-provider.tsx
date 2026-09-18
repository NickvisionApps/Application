import {
  createContext,
  ReactNode,
  useCallback,
  useContext,
  useEffect,
  useId,
  useMemo,
  useRef,
} from "react";

interface KeyboardShortcut {
  key: string;
  shift?: boolean;
  alt?: boolean;
  handler: () => void;
}

interface KeyboardShortcutProviderProps {
  children: ReactNode;
}

interface KeyboardShortcutProviderState {
  registerShortcut: (id: string, shortcut: KeyboardShortcut) => void;
  unregisterShortcut: (id: string) => void;
}

const KeyboardShortcutProviderContext = createContext<
  KeyboardShortcutProviderState | undefined
>(undefined);

export function KeyboardShortcutProvider({
  children,
  ...props
}: KeyboardShortcutProviderProps) {
  const shortcuts = useRef<Map<string, KeyboardShortcut>>(new Map());

  const registerShortcut = useCallback(
    (id: string, shortcut: KeyboardShortcut) => {
      shortcuts.current.set(id, shortcut);
    },
    [],
  );

  const unregisterShortcut = useCallback((id: string) => {
    shortcuts.current.delete(id);
  }, []);

  useEffect(() => {
    const handleKeyDown = (event: KeyboardEvent) => {
      if (!(event.metaKey || event.ctrlKey)) {
        return;
      }
      for (const shortcut of shortcuts.current.values()) {
        if (
          Boolean(shortcut.shift) !== event.shiftKey ||
          Boolean(shortcut.alt) !== event.altKey
        ) {
          continue;
        }
        const matchesKey = shortcut.alt
          ? event.code === `Key${shortcut.key.toUpperCase()}`
          : event.key.toLowerCase() === shortcut.key;
        if (matchesKey) {
          event.preventDefault();
          shortcut.handler();
          return;
        }
      }
    };

    window.addEventListener("keydown", handleKeyDown);
    return () => window.removeEventListener("keydown", handleKeyDown);
  }, []);

  return (
    <KeyboardShortcutProviderContext.Provider
      {...props}
      value={useMemo<KeyboardShortcutProviderState>(
        () => ({registerShortcut, unregisterShortcut}),
        [registerShortcut, unregisterShortcut],
      )}
    >
      {children}
    </KeyboardShortcutProviderContext.Provider>
  );
}

export function useKeyboardShortcut(
  key: string,
  handler: () => void,
  options?: {shift?: boolean; alt?: boolean; enabled?: boolean},
) {
  const context = useContext(KeyboardShortcutProviderContext);
  if (!context) {
    throw new Error(
      "useKeyboardShortcut must be used with a KeyboardShortcutProvider",
    );
  }
  const {registerShortcut, unregisterShortcut} = context;
  const id = useId();
  const handlerRef = useRef(handler);
  handlerRef.current = handler;
  const shift = options?.shift ?? false;
  const alt = options?.alt ?? false;
  const enabled = options?.enabled ?? true;

  useEffect(() => {
    if (!enabled) {
      return;
    }
    registerShortcut(id, {
      key,
      shift,
      alt,
      handler: () => handlerRef.current(),
    });
    return () => unregisterShortcut(id);
  }, [id, key, shift, alt, enabled, registerShortcut, unregisterShortcut]);
}
