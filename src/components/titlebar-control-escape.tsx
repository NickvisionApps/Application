import {ReactNode, useEffect, useRef, useState} from "react";
import {createPortal} from "react-dom";

interface TitlebarControlEscapeProps {
  children: ReactNode;
  renderAbsolute?: boolean;
}

export function TitlebarControlEscape({
  children,
  renderAbsolute = false,
}: TitlebarControlEscapeProps) {
  const anchorRef = useRef<HTMLDivElement>(null);
  const [rect, setRect] = useState<DOMRect | null>(null);
  const [target, setTarget] = useState<HTMLElement | null>(null);

  useEffect(() => {
    const targetRef = {current: null as HTMLElement | null};

    let frame: number;
    const update = () => {
      if (!targetRef.current) {
        const found = document.querySelector<HTMLElement>(
          "[data-tauri-decorum-tb]",
        );
        if (found) {
          targetRef.current = found;
          setTarget(found);
        }
      }

      const el = anchorRef.current;
      if (el) {
        const next = el.getBoundingClientRect();
        setRect((prev) =>
          prev &&
          prev.top === next.top &&
          prev.left === next.left &&
          prev.width === next.width &&
          prev.height === next.height
            ? prev
            : next,
        );
      }
      frame = requestAnimationFrame(update);
    };
    frame = requestAnimationFrame(update);

    return () => {
      cancelAnimationFrame(frame);
    };
  }, []);

  return (
    <>
      <div
        ref={anchorRef}
        className={
          renderAbsolute
            ? "absolute invisible pointer-events-none"
            : "invisible"
        }
      >
        {children}
      </div>
      {target &&
        rect &&
        createPortal(
          <div
            className="fixed z-1"
            style={{
              top: rect.top,
              left: rect.left,
              width: rect.width,
              height: rect.height,
            }}
          >
            {children}
          </div>,
          target,
        )}
    </>
  );
}
