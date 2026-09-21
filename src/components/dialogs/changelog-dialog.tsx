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
import {ScrollArea} from "@/components/ui/scroll-area.tsx";
import {useDialog} from "@/lib/contexts/dialog-context.ts";
import {useProductInfo} from "@/lib/contexts/product-info-context.ts";
import {useTranslation} from "@/lib/contexts/translation-context.ts";
import {Streamdown} from "streamdown";

export function ChangelogDialog() {
  const {productInfo} = useProductInfo();
  const {closeDialog} = useDialog();
  const {_g, _p} = useTranslation();

  const changelog = `- Initial Release
- Rewrote app in Rust and Tauri`;

  return (
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
          <DialogTitle>{_g("Changelog")}</DialogTitle>
          {productInfo.version && (
            <DialogDescription>{productInfo.version}</DialogDescription>
          )}
        </DialogHeader>
        <VStack className="min-h-0 flex-1 overflow-hidden">
          <ScrollArea className="flex-1">
            <Streamdown>{changelog}</Streamdown>
          </ScrollArea>
        </VStack>
        <DialogFooter>
          <DialogClose render={<Button variant="outline" />}>
            {_p("Dialog", "Close")}
          </DialogClose>
        </DialogFooter>
      </DialogContent>
    </Dialog>
  );
}
