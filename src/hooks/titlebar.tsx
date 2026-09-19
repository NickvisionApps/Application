import {useTitlebar} from "@/lib/titlebar-provider.tsx";
import {ReactNode, useEffect} from "react";

export function useTitlebarControls(content: ReactNode) {
  const {setContent} = useTitlebar();

  useEffect(() => {
    setContent(content);
    return () => setContent(null);
  }, [content, setContent]);
}
