import {invoke} from "@tauri-apps/api/core";
import {
  getCurrentWindow,
  LogicalPosition,
  LogicalSize,
} from "@tauri-apps/api/window";
import {ReactNode, useCallback, useEffect, useMemo, useState} from "react";

import {
  Configuration,
  ConfigurationProviderContext,
  ConfigurationProviderState,
} from "@/lib/configuration-context.ts";

interface ConfigurationProviderProps {
  children: ReactNode;
}

const DefaultConfiguration: Configuration = {
  allowPreviewUpdates: false,
  automaticallyCheckForUpdates: true,
  theme: 2,
  translationLanguage: "",
  windowGeometry: {
    x: 10,
    y: 10,
    width: 800,
    height: 600,
    isMaximized: false,
  },
};

export function ConfigurationProvider({
  children,
  ...props
}: ConfigurationProviderProps) {
  const [configuration, setConfiguration] =
    useState<Configuration>(DefaultConfiguration);

  useEffect(() => {
    async function startup() {
      setConfiguration(await invoke("get_configuration"));
    }

    void startup();
  }, []);

  useEffect(() => {
    async function applySize() {
      const window = getCurrentWindow();
      if (configuration.windowGeometry.isMaximized) {
        await window.maximize();
      } else {
        await window.setPosition(
          new LogicalPosition(
            configuration.windowGeometry.x,
            configuration.windowGeometry.y,
          ),
        );
        await window.setSize(
          new LogicalSize(
            configuration.windowGeometry.width,
            configuration.windowGeometry.height,
          ),
        );
      }
    }

    void applySize();
  }, [
    configuration.windowGeometry.x,
    configuration.windowGeometry.y,
    configuration.windowGeometry.width,
    configuration.windowGeometry.height,
    configuration.windowGeometry.isMaximized,
  ]);

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
        const window = getCurrentWindow();
        if ((await window.isMaximized()) || (await window.isFullscreen())) {
          newConfiguration = {
            ...newConfiguration,
            windowGeometry: {
              x: 10,
              y: 10,
              width: 800,
              height: 600,
              isMaximized: true,
            },
          };
        } else {
          const scale = await window.scaleFactor();
          const size = (await window.innerSize()).toLogical(scale);
          const position = (await window.outerPosition()).toLogical(scale);
          newConfiguration = {
            ...newConfiguration,
            windowGeometry: {
              x: Math.floor(position.x),
              y: Math.floor(position.y),
              width: Math.floor(size.width),
              height: Math.floor(size.height),
              isMaximized: false,
            },
          };
        }
        await invoke("set_configuration", {
          configuration: newConfiguration,
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
