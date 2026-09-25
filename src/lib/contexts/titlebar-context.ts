import {createContext, ReactNode, useContext} from "react";

export interface TitlebarProviderState {
  content: ReactNode;
  setContent: (content: ReactNode) => void;
}

export const TitlebarProviderContext = createContext<
  TitlebarProviderState | undefined
>(undefined);

export const useTitlebar = () => {
  const context = useContext(TitlebarProviderContext);
  if (!context) {
    throw new Error("useTitlebar must be used with a TitlebarProvider");
  }
  return context;
};
