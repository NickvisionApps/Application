import {
  Configuration,
  ConfigurationProviderContext,
  ConfigurationProviderState,
} from "@/lib/contexts/configuration-context.ts";
import {invoke} from "@tauri-apps/api/core";
import {ReactNode, useCallback, useEffect, useMemo, useState} from "react";

interface ConfigurationProviderProps {
  children: ReactNode;
}

const DefaultConfiguration: Configuration = {
  allowPreviewUpdates: false,
  automaticallyCheckForUpdates: true,
  theme: 2,
  translationLanguage: "",
};

export function ConfigurationProvider({
  children,
  ...props
}: ConfigurationProviderProps) {
  const [configuration, setConfiguration] =
    useState<Configuration>(DefaultConfiguration);

  useEffect(() => {
    async function startup() {
      setConfiguration(await invoke<Configuration>("get_configuration"));
    }

    void startup();
  }, []);

  useEffect(() => {
    const root = window.document.documentElement;
    root.classList.remove("light", "dark");
    if (configuration.theme === 2) {
      const systemTheme = window.matchMedia("(prefers-color-scheme: dark)")
        .matches
        ? "dark"
        : "light";
      root.classList.add(systemTheme);
    } else {
      root.classList.add(configuration.theme === 0 ? "light" : "dark");
    }
  }, [configuration.theme]);

  const handleSetConfiguration = useCallback(
    (newConfiguration: Configuration) => {
      async function saveConfiguration() {
        await invoke("set_configuration", {
          newConfiguration,
        });
        setConfiguration(newConfiguration);
      }

      void saveConfiguration();
    },
    [],
  );

  return (
    <ConfigurationProviderContext.Provider
      {...props}
      value={useMemo<ConfigurationProviderState>(
        () => ({
          configuration,
          setConfiguration: handleSetConfiguration,
        }),
        [configuration, handleSetConfiguration],
      )}
    >
      {children}
    </ConfigurationProviderContext.Provider>
  );
}
