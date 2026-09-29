import {createContext, ReactNode, useContext} from "react";

export type WindowButtonId = "minimize" | "maximize" | "close";

export interface TitlebarProviderState {
  content: ReactNode;
  setContent: (content: ReactNode) => void;
  side: "left" | "right";
  order: WindowButtonId[];
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
