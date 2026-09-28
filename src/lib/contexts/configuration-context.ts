import {createContext, useContext} from "react";

export type Theme = 0 | 1 | 2;

export interface Configuration {
  allowPreviewUpdates: boolean;
  automaticallyCheckForUpdates: boolean;
  theme: Theme;
  translationLanguage: string;
}

export interface ConfigurationProviderState {
  configuration: Configuration;
  setConfiguration: (newConfiguration: Configuration) => void;
}

export const ConfigurationProviderContext = createContext<
  ConfigurationProviderState | undefined
>(undefined);

export const useConfiguration = () => {
  const context = useContext(ConfigurationProviderContext);
  if (!context) {
    throw new Error(
      "useConfiguration must be used with a ConfigurationProvider",
    );
  }
  return context;
};
