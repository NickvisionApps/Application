import {ReactNode, useCallback, useMemo, useState} from "react";

import {ChangelogDialog} from "@/components/dialogs/changelog-dialog.tsx";
import {CreditsDialog} from "@/components/dialogs/credits-dialog.tsx";
import {DebuggingDialog} from "@/components/dialogs/debugging-dialog.tsx";
import {SettingsDialog} from "@/components/dialogs/settings-dialog.tsx";
import {
  DialogName,
  DialogProviderContext,
  DialogProviderState,
} from "@/lib/contexts/dialog-context.ts";

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
      {dialog === "credits" && <CreditsDialog />}
      {dialog === "debugging" && <DebuggingDialog />}
      {dialog === "settings" && <SettingsDialog />}
      {children}
    </DialogProviderContext.Provider>
  );
}
