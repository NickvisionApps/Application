import {toast} from "@/components/ui/toast.tsx";

import {
  Configuration,
  ConfigurationProviderContext,
  ConfigurationProviderState,
} from "@/lib/contexts/configuration-context.ts";
import {updateVibrancy} from "@nickvisionapps/plugin-window-integration";
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
  const [initialSystemDark] = useState(
    () => window.matchMedia("(prefers-color-scheme: dark)").matches,
  );

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
        configuration.theme === 2
          ? platform() === "linux"
            ? initialSystemDark
            : mql.matches
          : configuration.theme === 1;
      root.classList.toggle("dark", dark);
      root.classList.toggle("light", !dark);
      void getCurrentWindow().setTheme(
        configuration.theme === 2
          ? platform() === "linux"
            ? dark
              ? "dark"
              : "light"
            : null
          : configuration.theme === 1
            ? "dark"
            : "light",
      );
      void updateVibrancy(dark);
    }

    applyTheme();
    if (configuration.theme !== 2 || platform() === "linux") {
      return;
    }
    mql.addEventListener("change", applyTheme);
    return () => {
      mql.removeEventListener("change", applyTheme);
    };
  }, [configuration.theme, initialSystemDark]);

  const handleSetConfiguration = useCallback(
    (newConfiguration: Configuration) => {
      async function saveConfiguration() {
        try {
          await invoke("set_configuration", {
            newConfiguration,
          });
          setConfiguration(newConfiguration);
        } catch (error) {
          toast.add({
            type: "error",
            title: "Error",
            description: String(error),
          });
        }
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
