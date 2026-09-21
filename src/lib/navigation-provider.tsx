import {ReactNode, useMemo, useState} from "react";

import {
  NavigationProviderContext,
  NavigationProviderState,
  Page,
} from "@/lib/navigation-context.ts";

interface NavigationProviderProps {
  children: ReactNode;
}

export function NavigationProvider({
  children,
  ...props
}: NavigationProviderProps) {
  const [page, setPage] = useState<Page>("home");

  return (
    <NavigationProviderContext.Provider
      {...props}
      value={useMemo<NavigationProviderState>(
        () => ({
          page,
          setPage,
        }),
        [page],
      )}
    >
      {children}
    </NavigationProviderContext.Provider>
  );
}
