import {VStack} from "@/components/layout/stack.tsx";
import {
  Dialog,
  DialogContent,
  DialogHeader,
  DialogTitle,
} from "@/components/ui/dialog.tsx";
import {ScrollArea} from "@/components/ui/scroll-area.tsx";
import {useDialog} from "@/lib/contexts/dialog-context.ts";
import {useTranslation} from "@/lib/contexts/translation-context.ts";
import {Streamdown} from "streamdown";

export function CreditsDialog() {
  const {closeDialog} = useDialog();
  const {_g} = useTranslation();

  const credits = `**Developers**
- [Nick Logozzo](https://github.com/nlogozzo)
- [Contributors on GitHub ❤️](https://github.com/NickvisionApps/Application/graphs/contributors)

**Designers**
- [Nick Logozzo](https://github.com/nlogozzo)
- [Fyodor Sobolev](https://github.com/fysobolev)
- [DaPigGuy](https://github.com/DaPigGuy)

**Artists**
- [David Lapshin](https://github.com/daudix)`;

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
          <DialogTitle>{_g("Credits")}</DialogTitle>
        </DialogHeader>
        <VStack className="min-h-0 flex-1 overflow-hidden">
          <ScrollArea className="flex-1">
            <Streamdown
              linkSafety={{
                enabled: false,
              }}
            >
              {credits}
            </Streamdown>
          </ScrollArea>
        </VStack>
      </DialogContent>
    </Dialog>
  );
}
