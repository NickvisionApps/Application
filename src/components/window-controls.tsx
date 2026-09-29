import {
  useLinuxButtonLayout,
  type WindowButtonId,
} from "@/hooks/use-linux-button-layout.ts";
import {useWindowMaximized} from "@/hooks/use-window-maximized.ts";
import {useTranslation} from "@/lib/contexts/translation-context.ts";
import {getCurrentWindow} from "@tauri-apps/api/window";
import {platform} from "@tauri-apps/plugin-os";
import {ReactNode} from "react";

export function WindowControls() {
  const window = getCurrentWindow();
  const {_p} = useTranslation();
  const maximized = useWindowMaximized();
  const {order, side} = useLinuxButtonLayout();

  const buttons: Record<WindowButtonId, ReactNode> = {
    minimize: (
      <button
        key="minimize"
        aria-label={_p("TitleBar", "Minimize")}
        onClick={() => void window.minimize()}
      >
        <svg viewBox="0 0 10 10">
          <path d="M0 5h10" stroke="currentColor" strokeWidth="1" />
        </svg>
      </button>
    ),
    maximize: (
      <button
        key="maximize"
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
    ),
    close: (
      <button
        key="close"
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
    ),
  };

  return (
    <div
      className="window-controls"
      data-platform={platform()}
      data-side={side}
    >
      {order.map((id) => buttons[id])}
    </div>
  );
}
