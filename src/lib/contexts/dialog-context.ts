import {createContext, useContext} from "react";

export type DialogName =
  "none" | "changelog" | "close" | "credits" | "debugging" | "settings";

export interface DialogProviderState {
  dialog: DialogName;
  openDialog: (dialog: DialogName) => void;
  closeDialog: () => void;
}

export const DialogProviderContext = createContext<
  DialogProviderState | undefined
>(undefined);

export const useDialog = () => {
  const context = useContext(DialogProviderContext);
  if (!context) {
    throw new Error("useDialog must be used with a DialogProvider");
  }
  return context;
};
