import {useEffect, useRef} from "react";

export function useKeyboardShortcut(
  key: string,
  handler: () => void,
  options?: {shift?: boolean; alt?: boolean; enabled?: boolean},
) {
  const handlerRef = useRef(handler);
  handlerRef.current = handler;
  const shift = options?.shift ?? false;
  const alt = options?.alt ?? false;
  const enabled = options?.enabled ?? true;

  useEffect(() => {
    if (!enabled) {
      return;
    }

    const handleKeyDown = (event: KeyboardEvent) => {
      if (!(event.metaKey || event.ctrlKey)) {
        return;
      }
      if (Boolean(shift) !== event.shiftKey || Boolean(alt) !== event.altKey) {
        return;
      }
      const matchesKey = alt
        ? event.code === `Key${key.toUpperCase()}`
        : event.key.toLowerCase() === key;
      if (matchesKey) {
        event.preventDefault();
        handlerRef.current();
      }
    };

    window.addEventListener("keydown", handleKeyDown);
    return () => window.removeEventListener("keydown", handleKeyDown);
  }, [key, shift, alt, enabled]);
}
