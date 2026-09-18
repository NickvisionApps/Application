import {
  createContext,
  ReactNode,
  useCallback,
  useContext,
  useMemo,
  useState,
} from "react";

import {ChangelogDialog} from "@/components/dialogs/changelog-dialog.tsx";
import {CreditsDialog} from "@/components/dialogs/credits-dialog.tsx";
import {DebuggingDialog} from "@/components/dialogs/debugging-dialog.tsx";
import {SettingsDialog} from "@/components/dialogs/settings-dialog.tsx";

export type DialogName =
  "none" | "changelog" | "credits" | "debugging" | "settings";

interface DialogProviderProps {
  children: ReactNode;
}

interface DialogProviderState {
  dialog: DialogName;
  openDialog: (dialog: DialogName) => void;
  closeDialog: () => void;
}

const DialogProviderContext = createContext<DialogProviderState | undefined>(
  undefined,
);

export function DialogProvider({children, ...props}: DialogProviderProps) {
  const [dialog, setDialog] = useState<DialogName>("none");

  const closeDialog = useCallback(() => {
    setDialog("none");
  }, []);

  return (
    <DialogProviderContext.Provider
      {...props}
      value={useMemo<DialogProviderState>(
        () => ({
          dialog,
          openDialog: setDialog,
          closeDialog,
        }),
        [dialog, closeDialog],
      )}
    >
      {dialog === "changelog" && <ChangelogDialog />}
      {dialog === "credits" && <CreditsDialog />}
      {dialog === "debugging" && <DebuggingDialog />}
      {dialog === "settings" && <SettingsDialog />}
      {children}
    </DialogProviderContext.Provider>
  );
}

export const useDialog = () => {
  const context = useContext(DialogProviderContext);
  if (!context) {
    throw new Error("useDialog must be used with a DialogProvider");
  }
  return context;
};
