import {VStack} from "@/components/layout/stack.tsx";
import {
  Dialog,
  DialogContent,
  DialogDescription,
  DialogHeader,
  DialogTitle,
} from "@/components/ui/dialog.tsx";
import {ScrollArea} from "@/components/ui/scroll-area.tsx";
import {Spinner} from "@/components/ui/spinner.tsx";
import {useDialog} from "@/lib/contexts/dialog-context.ts";
import {useTranslation} from "@/lib/contexts/translation-context.ts";
import {code} from "@streamdown/code";
import {invoke} from "@tauri-apps/api/core";
import {useEffect, useState} from "react";
import {Streamdown} from "streamdown";

let cachedDebuggingInformation: string | null = null;

export function DebuggingDialog() {
  const {closeDialog} = useDialog();
  const {_g} = useTranslation();
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
      <DialogContent className="flex max-h-[95vh] flex-col sm:max-w-lg">
        <DialogHeader>
          <DialogTitle>{_g("Debugging Information")}</DialogTitle>
          <DialogDescription>
            {_g("Send this information to the developer when reporting issues")}
          </DialogDescription>
        </DialogHeader>
        {debuggingInformation && (
          <VStack className="min-h-0 flex-1 overflow-hidden">
            <ScrollArea className="h-full w-full">
              <Streamdown
                codeBlockMaxHeight={0}
                tableMaxHeight={0}
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
            </ScrollArea>
          </VStack>
        )}
        {!debuggingInformation && <Spinner />}
      </DialogContent>
    </Dialog>
  );
}
