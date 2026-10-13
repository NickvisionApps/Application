import {VStack} from "@/components/layout/stack.tsx";
import {Button} from "@/components/ui/button.tsx";
import {
  Dialog,
  DialogContent,
  DialogDescription,
  DialogFooter,
  DialogHeader,
  DialogTitle,
} from "@/components/ui/dialog.tsx";
import {ScrollArea} from "@/components/ui/scroll-area.tsx";
import {Spinner} from "@/components/ui/spinner.tsx";
import {useDialog} from "@/lib/contexts/dialog-context.ts";
import {useTranslation} from "@/lib/contexts/translation-context.ts";
import {invoke} from "@tauri-apps/api/core";
import {Check, Copy} from "lucide-react";
import {useEffect, useState} from "react";

let cachedDebuggingInformation: string | null = null;

export function DebuggingDialog() {
  const {closeDialog} = useDialog();
  const {_g} = useTranslation();
  const [copied, setCopied] = useState(false);
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
            <ScrollArea className="h-full w-full rounded-lg border bg-muted/50">
              <VStack gap={4} className="p-4">
                {debuggingInformation
                  .replace(/```/g, "")
                  .trim()
                  .split("\n\n")
                  .map((group, groupIndex) => (
                    <VStack key={groupIndex} gap={2}>
                      {group.split("\n").map((line) => (
                        <div
                          key={line}
                          className="grid grid-cols-[7.5rem_1fr] items-baseline gap-3"
                        >
                          <span className="text-muted-foreground">
                            {line.slice(0, line.indexOf(": "))}
                          </span>
                          <span className="font-mono text-xs break-all select-text">
                            {line.slice(line.indexOf(": ") + 2)}
                          </span>
                        </div>
                      ))}
                    </VStack>
                  ))}
              </VStack>
            </ScrollArea>
          </VStack>
        )}
        {!debuggingInformation && <Spinner />}
        {debuggingInformation && (
          <DialogFooter>
            <Button
              variant="outline"
              onClick={() => {
                void navigator.clipboard.writeText(
                  debuggingInformation.replace(/```/g, "").trim(),
                );
                setCopied(true);
                setTimeout(() => setCopied(false), 2000);
              }}
            >
              {copied ? <Check /> : <Copy />}
              <span>{_g("Copy")}</span>
            </Button>
          </DialogFooter>
        )}
      </DialogContent>
    </Dialog>
  );
}
