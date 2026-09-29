import {
  TitlebarProviderContext,
  TitlebarProviderState,
  WindowButtonId,
} from "@/lib/contexts/titlebar-context.ts";
import {invoke} from "@tauri-apps/api/core";
import {platform} from "@tauri-apps/plugin-os";
import {ReactNode, useEffect, useMemo, useState} from "react";

interface TitlebarProviderProps {
  children: ReactNode;
}

export function TitlebarProvider({children, ...props}: TitlebarProviderProps) {
  const [content, setContent] = useState<ReactNode>(null);
  const [side, setSide] = useState<"left" | "right">("right");
  const [order, setOrder] = useState<WindowButtonId[]>([
    "minimize",
    "maximize",
    "close",
  ]);

  useEffect(() => {
    if (platform() !== "linux") {
      return;
    }

    async function startup() {
      function parseTokens(tokens: string): WindowButtonId[] {
        return tokens
          .split(",")
          .map((token) => token.trim())
          .filter(
            (token): token is WindowButtonId =>
              token === "minimize" || token === "maximize" || token === "close",
          );
      }

      const raw = await invoke<string | null>("get_linux_button_layout");
      if (!raw) {
        return;
      }
      const [leftRaw, rightRaw] = raw.includes(":")
        ? raw.split(":")
        : ["", raw];
      const left = parseTokens(leftRaw);
      const right = parseTokens(rightRaw);
      if (left.length > 0) {
        setSide("left");
        setOrder(left);
      } else if (right.length > 0) {
        setSide("right");
        setOrder(right);
      }
    }

    void startup();
  }, []);

  return (
    <TitlebarProviderContext.Provider
      {...props}
      value={useMemo<TitlebarProviderState>(
        () => ({content, setContent, side, order}),
        [content, side, order],
      )}
    >
      {children}
    </TitlebarProviderContext.Provider>
  );
}
