import {
  Configuration,
  ConfigurationProviderContext,
  ConfigurationProviderState,
} from "@/lib/contexts/configuration-context.ts";
import {invoke} from "@tauri-apps/api/core";
import {getCurrentWindow} from "@tauri-apps/api/window";
import {platform} from "@tauri-apps/plugin-os";
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
    const mql = window.matchMedia("(prefers-color-scheme: dark)");

    function applyTheme() {
      const dark =
        configuration.theme === 2 ? mql.matches : configuration.theme === 1;
      root.classList.toggle("dark", dark);
      root.classList.toggle("light", !dark);
    }

    applyTheme();
    if (configuration.theme !== 2 || platform() !== "linux") {
      void getCurrentWindow().setTheme(
        configuration.theme === 2
          ? null
          : configuration.theme === 1
            ? "dark"
            : "light",
      );
    }
    if (configuration.theme !== 2) {
      return;
    }
    mql.addEventListener("change", applyTheme);
    return () => {
      mql.removeEventListener("change", applyTheme);
    };
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
