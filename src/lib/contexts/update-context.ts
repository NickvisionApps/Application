import {createContext, useContext} from "react";

export interface UpdateDownloadProgress {
  downloaded: number;
  total: number;
}

export interface UpdateInformation {
  version: string;
  changelog: string | null;
  date: number | null;
}

export interface UpdateProviderState {
  checkingForUpdates: boolean;
  installingUpdate: boolean;
  updateDownloadProgress: UpdateDownloadProgress | null;
  updateInformation: UpdateInformation | null;
  checkForUpdates: () => Promise<void>;
  installUpdate: () => Promise<void>;
}

export const UpdateProviderContext = createContext<
  UpdateProviderState | undefined
>(undefined);

export const useUpdate = () => {
  const context = useContext(UpdateProviderContext);
  if (!context) {
    throw new Error("useUpdate must be used with a UpdateProvider");
  }
  return context;
};
