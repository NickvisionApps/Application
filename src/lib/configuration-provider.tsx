import {invoke} from "@tauri-apps/api/core";
import {getCurrentWindow, LogicalPosition, LogicalSize} from "@tauri-apps/api/window";
import {createContext, ReactNode, useCallback, useContext, useEffect, useMemo, useState} from "react";

type Theme = 0 | 1 | 2;

interface WindowGeometry {
  x: number,
  y: number,
  width: number,
  height: number,
  isMaximized: boolean
}

interface Configuration {
  allowPreviewUpdates: boolean,
  theme: Theme,
  translationLanguage: string,
  windowGeometry: WindowGeometry,
}

interface ConfigurationProviderProps {
  children: ReactNode
}

interface ConfigurationProviderState {
  configuration: Configuration,
  setConfiguration: (newConfiguration: Configuration) => void
}

const DefaultConfiguration: Configuration = {
  allowPreviewUpdates: false,
  theme: 2,
  translationLanguage: "",
  windowGeometry: {
    x: 10,
    y: 10,
    width: 800,
    height: 600,
    isMaximized: false
  }
};

const ConfigurationProviderContext = createContext<ConfigurationProviderState>({
  configuration: DefaultConfiguration,
  setConfiguration: () => null
});

export function ConfigurationProvider({children, ...props}: ConfigurationProviderProps) {
  const [configuration, setConfiguration] = useState<Configuration>(DefaultConfiguration);

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
        await window.setPosition(new LogicalPosition(configuration.windowGeometry.x, configuration.windowGeometry.y));
        await window.setSize(new LogicalSize(configuration.windowGeometry.width, configuration.windowGeometry.height));
      }
    }

    void applySize();
  }, [
    configuration.windowGeometry.x,
    configuration.windowGeometry.y,
    configuration.windowGeometry.width,
    configuration.windowGeometry.height,
    configuration.windowGeometry.isMaximized
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

  const handleSetConfiguration = useCallback((newConfiguration: Configuration) => {
    async function saveConfiguration() {
      const window = getCurrentWindow();
      if (await window.isMaximized() || await window.isFullscreen()) {
        newConfiguration = {
          ...newConfiguration,
          windowGeometry: {
            x: 10,
            y: 10,
            width: 800,
            height: 600,
            isMaximized: true
          }
        };
      } else {
        const scale = await window.scaleFactor();
        const size = (await window.outerSize()).toLogical(scale);
        const position = (await window.outerPosition()).toLogical(scale);
        newConfiguration = {
          ...newConfiguration,
          windowGeometry: {
            x: position.x,
            y: position.y,
            width: size.width,
            height: size.height,
            isMaximized: false
          }
        };
      }
      await invoke("set_configuration", {
        configuration: newConfiguration
      });
      setConfiguration(newConfiguration);
    }

    void saveConfiguration();
  }, []);

  return (
    <ConfigurationProviderContext.Provider {...props} value={useMemo<ConfigurationProviderState>(() => ({
      configuration,
      setConfiguration: handleSetConfiguration
    }), [configuration, handleSetConfiguration])}>
      {children}
    </ConfigurationProviderContext.Provider>
  )
}

export const useConfiguration = () => {
  const context = useContext(ConfigurationProviderContext);
  if (!context) {
    throw new Error("useConfiguration must be used with a ConfigurationProvider");
  }
  return context;
}