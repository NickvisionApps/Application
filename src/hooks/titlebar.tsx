import {useTitlebar} from "@/lib/contexts/titlebar-context.ts";
import {ReactNode, useEffect} from "react";

export function useTitlebarControls(content: ReactNode) {
  const {setContent} = useTitlebar();

  useEffect(() => {
    setContent(content);
    return () => setContent(null);
  }, [content, setContent]);
}
