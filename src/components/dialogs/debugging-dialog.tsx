import {code} from "@streamdown/code";
import {invoke} from "@tauri-apps/api/core";
import {useEffect, useState} from "react";
import {Streamdown} from "streamdown";

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
import {ScrollArea, ScrollBar} from "@/components/ui/scroll-area.tsx";
import {Spinner} from "@/components/ui/spinner.tsx";
import {useDialog} from "@/lib/dialog-context.ts";
import {useTranslation} from "@/lib/translation-context.ts";

let cachedDebuggingInformation: string | null = null;

export function DebuggingDialog() {
  const {closeDialog} = useDialog();
  const {_g, _p} = useTranslation();
  const [debuggingInformation, setDebuggingInformation] = useState(
    cachedDebuggingInformation ?? "",
  );

  useEffect(() => {
    if (cachedDebuggingInformation !== null) {
      return;
    }

    async function startup() {
      const fetchedDebuggingInformation = await invoke<string>(
        "get_debugging_information",
      );
      cachedDebuggingInformation = fetchedDebuggingInformation;
      setDebuggingInformation(fetchedDebuggingInformation);
    }

    void startup();
  }, []);

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
          <DialogTitle>{_g("Debugging Information")}</DialogTitle>
          <DialogDescription>
            {_g("Send this information to the developer when reporting issues")}
          </DialogDescription>
        </DialogHeader>
        {debuggingInformation && (
          <ScrollArea className="min-h-0 flex-1">
            <Streamdown
              plugins={{
                code: code,
              }}
              controls={{
                code: {
                  download: false,
                },
              }}
              translations={{
                copyCode: _g("Copy Code"),
              }}
            >
              {debuggingInformation}
            </Streamdown>
            <ScrollBar orientation="horizontal" />
          </ScrollArea>
        )}
        {!debuggingInformation && <Spinner />}
        <DialogFooter>
          <DialogClose render={<Button variant="outline" />}>
            {_p("Dialog", "Close")}
          </DialogClose>
        </DialogFooter>
      </DialogContent>
    </Dialog>
  );
}
