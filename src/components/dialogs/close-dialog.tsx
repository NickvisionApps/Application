import {
  AlertDialog,
  AlertDialogAction,
  AlertDialogCancel,
  AlertDialogContent,
  AlertDialogDescription,
  AlertDialogFooter,
  AlertDialogHeader,
  AlertDialogTitle,
} from "@/components/ui/alert-dialog.tsx";
import {useDialog} from "@/lib/contexts/dialog-context.ts";
import {useTranslation} from "@/lib/contexts/translation-context.ts";
import {invoke} from "@tauri-apps/api/core";
import {getCurrentWindow} from "@tauri-apps/api/window";

export function CloseDialog() {
  const {closeDialog} = useDialog();
  const {_g, _p} = useTranslation();

  return (
    <AlertDialog
      open={true}
      onOpenChange={(open) => {
        if (!open) {
          closeDialog();
        }
      }}
    >
      <AlertDialogContent>
        <AlertDialogHeader>
          <AlertDialogTitle>
            {_p("AppName", "Exit Application?")}
          </AlertDialogTitle>
          <AlertDialogDescription>
            {_g("Are you sure you want to exit?")}
          </AlertDialogDescription>
        </AlertDialogHeader>
        <AlertDialogFooter>
          <AlertDialogCancel>{_g("No")}</AlertDialogCancel>
          <AlertDialogAction
            onClick={() => {
              const window = getCurrentWindow();

              async function confirmClose() {
                await invoke("confirm_window_close");
                await window.close();
              }

              closeDialog();
              void confirmClose();
            }}
          >
            {_g("Yes")}
          </AlertDialogAction>
        </AlertDialogFooter>
      </AlertDialogContent>
    </AlertDialog>
  );
}
