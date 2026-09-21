import {ReactNode, useMemo, useState} from "react";

import {
  TitlebarProviderContext,
  TitlebarProviderState,
} from "@/lib/titlebar-context.ts";

interface TitlebarProviderProps {
  children: ReactNode;
}

export function TitlebarProvider({children, ...props}: TitlebarProviderProps) {
  const [content, setContent] = useState<ReactNode>(null);

  return (
    <TitlebarProviderContext.Provider
      {...props}
      value={useMemo<TitlebarProviderState>(
        () => ({content, setContent}),
        [content],
      )}
    >
      {children}
    </TitlebarProviderContext.Provider>
  );
}
