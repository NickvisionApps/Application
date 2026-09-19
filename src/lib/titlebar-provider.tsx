import {createContext, ReactNode, useContext, useMemo, useState} from "react";

interface TitlebarProviderProps {
  children: ReactNode;
}

interface TitlebarProviderState {
  content: ReactNode;
  setContent: (content: ReactNode) => void;
}

const TitlebarProviderContext = createContext<
  TitlebarProviderState | undefined
>(undefined);

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

export const useTitlebar = () => {
  const context = useContext(TitlebarProviderContext);
  if (!context) {
    throw new Error("useTitlebar must be used with a TitlebarProvider");
  }
  return context;
};
