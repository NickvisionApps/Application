import {Button} from "@/components/ui/button.tsx";
import {
  Dialog,
  DialogClose,
  DialogContent,
  DialogDescription,
  DialogFooter,
  DialogHeader,
  DialogTitle
} from "@/components/ui/dialog.tsx";
import {ScrollArea} from "@/components/ui/scroll-area.tsx";
import {useDialog} from "@/lib/dialog-provider.tsx";
import {useTranslation} from "@/lib/translation-provider.tsx";
import {invoke} from "@tauri-apps/api/core";
import {useEffect, useState} from "react";
import {Streamdown} from "streamdown";

let cachedVersion: string | null = null;

export function ChangelogDialog() {
  const {closeDialog} = useDialog();
  const {_g} = useTranslation();
  const [version, setVersion] = useState(cachedVersion ?? "");

  const changelog = `- Initial Release
- Rewrote app in Rust and Tauri`;

  useEffect(() => {
    if (cachedVersion !== null) {
      return;
    }

    async function startup() {
      const fetchedVersion = await invoke<string>("get_version");
      cachedVersion = fetchedVersion;
      setVersion(fetchedVersion);
    }

    void startup();
  }, []);

  return (
    <Dialog open={true} onOpenChange={(open) => {
      if (!open) {
        closeDialog();
      }
    }}>
      <DialogContent className="flex max-h-[95vh] flex-col">
        <DialogHeader>
          <DialogTitle>{_g("Changelog")}</DialogTitle>
          {version && <DialogDescription>{version}</DialogDescription>}
        </DialogHeader>
        <ScrollArea className="min-h-0 flex-1">
          <Streamdown>{changelog}</Streamdown>
        </ScrollArea>
        <DialogFooter>
          <DialogClose render={<Button variant="outline"/>}>{_g("Close")}</DialogClose>
        </DialogFooter>
      </DialogContent>
    </Dialog>
  )
}