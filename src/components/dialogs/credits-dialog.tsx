import {Streamdown} from "streamdown";

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
import {useDialog} from "@/lib/dialog-provider.tsx";
import {useTranslation} from "@/lib/translation-provider.tsx";

export function CreditsDialog() {
  const {closeDialog} = useDialog();
  const {_g, _p} = useTranslation();

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
        <ScrollArea className="min-h-0 flex-1">
          <Streamdown
            linkSafety={{
              enabled: false,
            }}
          >
            {credits}
          </Streamdown>
        </ScrollArea>
        <DialogFooter>
          <DialogClose render={<Button variant="outline" />}>
            {_p("Dialog", "Close")}
          </DialogClose>
        </DialogFooter>
      </DialogContent>
    </Dialog>
  );
}
