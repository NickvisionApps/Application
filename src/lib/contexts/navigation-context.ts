import {createContext, useContext} from "react";

export type Page = "home" | "folder";

export interface NavigationProviderState {
  page: Page;
  setPage: (newPage: Page) => void;
  canGoBack: boolean;
  canGoForward: boolean;
  goBack: () => void;
  goForward: () => void;
}

export const NavigationProviderContext = createContext<
  NavigationProviderState | undefined
>(undefined);

export const useNavigation = () => {
  const context = useContext(NavigationProviderContext);
  if (!context) {
    throw new Error("useNavigation must be used with a NavigationProvider");
  }
  return context;
};
