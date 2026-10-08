import {useEffect, useRef} from "react";

export interface KeyboardShortcutOptions {
  control?: boolean;
  shift?: boolean;
  alt?: boolean;
  enabled?: boolean;
}

export function useKeyboardShortcut(
  key: string,
  handler: () => void,
  options?: KeyboardShortcutOptions,
) {
  const handlerRef = useRef(handler);
  handlerRef.current = handler;
  const control = options?.control ?? true;
  const shift = options?.shift ?? false;
  const alt = options?.alt ?? false;
  const enabled = options?.enabled ?? true;

  useEffect(() => {
    if (!enabled) {
      return;
    }

    const handleKeyDown = (event: KeyboardEvent) => {
      if (
        (control && !(event.metaKey || event.ctrlKey)) ||
        shift !== event.shiftKey ||
        alt !== event.altKey
      ) {
        return;
      }
      const matchesKey = alt
        ? event.code === `Key${key.toUpperCase()}`
        : event.key.toLowerCase() === key.toLowerCase();
      if (matchesKey) {
        event.preventDefault();
        handlerRef.current();
      }
    };

    window.addEventListener("keydown", handleKeyDown);
    return () => window.removeEventListener("keydown", handleKeyDown);
  }, [key, shift, alt, enabled]);
}
