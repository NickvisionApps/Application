import {platform} from "@tauri-apps/plugin-os";
import {cn} from "cn";
import {ReactNode, useEffect} from "react";

import {HStack} from "@/components/layout/stack.tsx";
import {useTitlebar} from "@/lib/contexts/titlebar-context.ts";

export function useTitlebarControls(content: ReactNode) {
  const {setContent, side} = useTitlebar();
  const hasTrailingControls = platform() === "macos" || side !== "left";

  useEffect(() => {
    setContent(
      <HStack
        align="center"
        justify="end"
        gap={2}
        className="min-w-0 flex-1 mt-2 mr-2"
      >
        {content}
      </HStack>,
    );
    return () => {
      setContent(null);
    };
  }, [content, setContent, hasTrailingControls]);
}
