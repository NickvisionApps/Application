import {Button} from "@/components/ui/button.tsx";
import {Dialog, DialogClose, DialogContent, DialogFooter, DialogHeader, DialogTitle} from "@/components/ui/dialog.tsx";
import {useConfiguration} from "@/lib/configuration-provider.tsx";
import {useDialog} from "@/lib/dialog-provider.tsx";
import {useTranslation} from "@/lib/translation-provider.tsx";
import {invoke} from "@tauri-apps/api/core";
import {useEffect, useState} from "react";

export function SettingsDialog() {
  const {closeDialog} = useDialog();
  const {configuration, setConfiguration} = useConfiguration();
  const {_g} = useTranslation();
  const [availableLanguages, setAvailableLanguages] = useState<string[]>([]);

  useEffect(() => {
    async function startup() {
      setAvailableLanguages(await invoke("get_available_translation_languages"));
    }

    void startup();
  }, []);

  return (
    <Dialog open={true} onOpenChange={(open) => {
      if (!open) {
        closeDialog();
      }
    }}>
      <DialogContent>
        <DialogHeader>
          <DialogTitle>{_g("Settings")}</DialogTitle>
        </DialogHeader>
        <DialogFooter>
          <DialogClose render={<Button variant="outline"/>}>{_g("Close")}</DialogClose>
        </DialogFooter>
      </DialogContent>
    </Dialog>
  )
}