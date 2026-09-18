import {createContext, ReactNode, useContext, useMemo, useState} from "react";

export type Page = "home" | "folder";

interface NavigationProviderProps {
  children: ReactNode
}

interface NavigationProviderState {
  page: Page,
  setPage: (newPage: Page) => void
}

const NavigationProviderContext = createContext<NavigationProviderState>({
  page: "home",
  setPage: () => null
});

export function NavigationProvider({children, ...props}: NavigationProviderProps) {
  const [page, setPage] = useState<Page>("home");

  return (
    <NavigationProviderContext.Provider {...props} value={useMemo<NavigationProviderState>(() => ({
      page,
      setPage
    }), [page])}>
      {children}
    </NavigationProviderContext.Provider>
  )
}

export const useNavigation = () => {
  const context = useContext(NavigationProviderContext);
  if (!context) {
    throw new Error("useNavigation must be used with a NavigationProvider");
  }
  return context;
}