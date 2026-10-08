import {ReactNode, useCallback, useMemo, useState} from "react";

import {
  NavigationProviderContext,
  NavigationProviderState,
  Page,
} from "@/lib/contexts/navigation-context.ts";

interface NavigationProviderProps {
  children: ReactNode;
}

interface NavigationState {
  history: Page[];
  index: number;
}

const DefaultNavigationState: NavigationState = {
  history: ["home"],
  index: 0,
};

export function NavigationProvider({
  children,
  ...props
}: NavigationProviderProps) {
  const [state, setState] = useState<NavigationState>(DefaultNavigationState);

  const setPage = useCallback((newPage: Page) => {
    setState((prev) => {
      if (prev.history[prev.index] === newPage) {
        return prev;
      }
      const history = [...prev.history.slice(0, prev.index + 1), newPage];
      return {history, index: history.length - 1};
    });
  }, []);

  const goBack = useCallback(() => {
    setState((prev) =>
      prev.index > 0 ? {...prev, index: prev.index - 1} : prev,
    );
  }, []);

  const goForward = useCallback(() => {
    setState((prev) =>
      prev.index < prev.history.length - 1
        ? {...prev, index: prev.index + 1}
        : prev,
    );
  }, []);

  return (
    <NavigationProviderContext.Provider
      {...props}
      value={useMemo<NavigationProviderState>(
        () => ({
          page: state.history[state.index],
          setPage,
          canGoBack: state.index > 0,
          canGoForward: state.index < state.history.length - 1,
          goBack,
          goForward,
        }),
        [state, setPage, goBack, goForward],
      )}
    >
      {children}
    </NavigationProviderContext.Provider>
  );
}
