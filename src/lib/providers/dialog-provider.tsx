import {ChangelogDialog} from "@/components/dialogs/changelog-dialog.tsx";
import {CloseDialog} from "@/components/dialogs/close-dialog.tsx";
import {CreditsDialog} from "@/components/dialogs/credits-dialog.tsx";
import {DebuggingDialog} from "@/components/dialogs/debugging-dialog.tsx";
import {SettingsDialog} from "@/components/dialogs/settings-dialog.tsx";
import {
  DialogName,
  DialogProviderContext,
  DialogProviderState,
} from "@/lib/contexts/dialog-context.ts";
import {ReactNode, useCallback, useMemo, useState} from "react";

interface DialogProviderProps {
  children: ReactNode;
}

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
      {dialog === "close" && <CloseDialog />}
      {dialog === "credits" && <CreditsDialog />}
      {dialog === "debugging" && <DebuggingDialog />}
      {dialog === "settings" && <SettingsDialog />}
      {children}
    </DialogProviderContext.Provider>
  );
}
