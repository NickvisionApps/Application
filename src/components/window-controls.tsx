import {useTranslation} from "@/lib/contexts/translation-context.ts";
import {getCurrentWindow} from "@tauri-apps/api/window";
import {platform} from "@tauri-apps/plugin-os";
import {useEffect, useState} from "react";

export function WindowControls() {
  const window = getCurrentWindow();
  const {_p} = useTranslation();
  const [maximized, setMaximized] = useState(false);

  useEffect(() => {
    let resizeFn: () => void;

    async function startup() {
      setMaximized(await window.isMaximized());
      resizeFn = await window.onResized(async (_) => {
        setMaximized(await window.isMaximized());
      });
    }

    void startup();

    return () => {
      if (resizeFn) {
        resizeFn();
      }
    };
  }, []);

  return (
    <div className="window-controls" data-platform={platform()}>
      <button
        aria-label={_p("TitleBar", "Minimize")}
        onClick={() => void window.minimize()}
      >
        <svg viewBox="0 0 10 10">
          <path d="M0 5h10" stroke="currentColor" strokeWidth="1" />
        </svg>
      </button>
      <button
        aria-label={
          maximized ? _p("TitleBar", "Restore") : _p("TitleBar", "Maximize")
        }
        onClick={() => void window.toggleMaximize()}
      >
        <svg viewBox="0 0 10 10">
          <rect
            x="0.5"
            y="0.5"
            width="9"
            height="9"
            rx="1"
            fill="none"
            stroke="currentColor"
            strokeWidth="1"
          />
        </svg>
      </button>
      <button
        className="close"
        aria-label={_p("TitleBar", "Close")}
        onClick={() => void window.close()}
      >
        <svg viewBox="0 0 10 10">
          <path
            d="M0 0l10 10M10 0L0 10"
            stroke="currentColor"
            strokeWidth="1"
          />
        </svg>
      </button>
    </div>
  );
}
