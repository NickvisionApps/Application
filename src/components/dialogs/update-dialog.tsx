import {VStack} from "@/components/layout/stack.tsx";
import {Alert, AlertDescription} from "@/components/ui/alert.tsx";
import {Button} from "@/components/ui/button.tsx";
import {
  Dialog,
  DialogClose,
  DialogContent,
  DialogFooter,
  DialogHeader,
  DialogTitle,
} from "@/components/ui/dialog.tsx";
import {ScrollArea} from "@/components/ui/scroll-area.tsx";
import {useDialog} from "@/lib/contexts/dialog-context.ts";
import {useTranslation} from "@/lib/contexts/translation-context.ts";
import {useUpdate} from "@/lib/contexts/update-context.ts";
import {platform} from "@tauri-apps/plugin-os";
import {TriangleAlert} from "lucide-react";
import {Streamdown} from "streamdown";

export function UpdateDialog() {
  const {closeDialog} = useDialog();
  const {updateInformation, installUpdate} = useUpdate();
  const {_g, _p} = useTranslation();

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
          <DialogTitle>{_g("New Update")}</DialogTitle>
        </DialogHeader>
        <VStack gap={4} className="min-h-0 flex-1 overflow-hidden">
          <span className="text-sm text-muted-foreground">
            {updateInformation.version}
            {updateInformation.date &&
              ` · ${new Date(updateInformation.date * 1000).toLocaleDateString(
                undefined,
                {year: "numeric", month: "long", day: "numeric"},
              )}`}
          </span>
          {updateInformation.changelog && (
            <VStack gap={2} className="min-h-0 flex-1 overflow-hidden">
              <span className="text-xs font-medium text-muted-foreground">
                {_g("Changelog")}
              </span>
              <ScrollArea className="min-h-0 flex-1">
                <Streamdown>{updateInformation.changelog}</Streamdown>
              </ScrollArea>
            </VStack>
          )}
        </VStack>
        <Alert variant="destructive">
          <TriangleAlert />
          <AlertDescription>
            {_p(
              "AppName",
              "Application will restart to finish installing the update",
            )}
          </AlertDescription>
        </Alert>
        <DialogFooter>
          {platform() === "windows" && (
            <>
              <DialogClose
                render={
                  <Button
                    onClick={() => {
                      void installUpdate();
                    }}
                  >
                    {_g("Install")}
                  </Button>
                }
              />
              <DialogClose
                render={<Button variant="secondary">{_g("Cancel")}</Button>}
              />
            </>
          )}
          {platform() !== "windows" && (
            <>
              <DialogClose
                render={<Button variant="secondary">{_g("Cancel")}</Button>}
              />
              <DialogClose
                render={
                  <Button
                    onClick={() => {
                      void installUpdate();
                    }}
                  >
                    {_g("Install")}
                  </Button>
                }
              />
            </>
          )}
        </DialogFooter>
      </DialogContent>
    </Dialog>
  ) : (
    <></>
  );
}
