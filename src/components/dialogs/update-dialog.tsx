import {VStack} from "@/components/layout/stack.tsx";
import {Button} from "@/components/ui/button.tsx";
import {
  Dialog,
  DialogClose,
  DialogContent,
  DialogDescription,
  DialogFooter,
  DialogHeader,
  DialogTitle,
} from "@/components/ui/dialog.tsx";
import {useDialog} from "@/lib/contexts/dialog-context.ts";
import {useTranslation} from "@/lib/contexts/translation-context.ts";
import {useUpdate} from "@/lib/contexts/update-context.ts";
import {platform} from "@tauri-apps/plugin-os";

export function UpdateDialog() {
  const {closeDialog} = useDialog();
  const {updateInformation, installUpdate} = useUpdate();
  const {_g, _pf} = useTranslation();

  return updateInformation ? (
    <Dialog
      open={true}
      onOpenChange={(open) => {
        if (!open) {
          closeDialog();
        }
      }}
    >
      <DialogContent className="flex max-h-[95vh] flex-col">
        <DialogHeader>
          <DialogTitle>{_g("New Update!")}</DialogTitle>
          <DialogDescription>
            {_pf(
              "AppName",
              "Version {0} of Application is available for download and installation",
              [updateInformation.version],
            )}
          </DialogDescription>
          <VStack gap={4} className="min-h-0 flex-1">

          </VStack>
        </DialogHeader>
        <DialogFooter>
          {platform() === "windows" && (
            <>
              <Button>{_g("Install")}</Button>
              <DialogClose render={<Button variant="secondary">{_g("Cancel")}</Button>}/>
            </>
          )}
          {platform() !== "windows" && (
            <>
              <DialogClose render={<Button variant="secondary">{_g("Cancel")}</Button>}/>
              <Button>{_g("Install")}</Button>
            </>
          )}
        </DialogFooter>
      </DialogContent>
    </Dialog>
  ) : (
    <></>
  );
}
