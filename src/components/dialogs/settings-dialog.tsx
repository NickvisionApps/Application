import {VStack} from "@/components/layout/stack.tsx";
import {Button} from "@/components/ui/button.tsx";
import {Dialog, DialogClose, DialogContent, DialogFooter, DialogHeader, DialogTitle,} from "@/components/ui/dialog.tsx";
import {Field, FieldGroup, FieldLabel} from "@/components/ui/field.tsx";
import {ScrollArea} from "@/components/ui/scroll-area.tsx";
import {Select, SelectContent, SelectGroup, SelectItem, SelectTrigger, SelectValue,} from "@/components/ui/select.tsx";
import {Switch} from "@/components/ui/switch.tsx";
import {Tabs, TabsList, TabsTrigger} from "@/components/ui/tabs.tsx";
import {useConfiguration} from "@/lib/configuration-provider.tsx";
import {useDialog} from "@/lib/dialog-provider.tsx";
import {useTranslation} from "@/lib/translation-provider.tsx";
import {invoke} from "@tauri-apps/api/core";
import {Settings2, Wrench} from "lucide-react";
import {useEffect, useState} from "react";

type SettingsDialogPage = "general" | "advanced";

export function SettingsDialog() {
  const {closeDialog} = useDialog();
  const {configuration, setConfiguration} = useConfiguration();
  const {_g, _p} = useTranslation();
  const [availableLanguages, setAvailableLanguages] = useState<string[]>([]);
  const [page, setPage] = useState<SettingsDialogPage>("general");

  useEffect(() => {
    async function startup() {
      setAvailableLanguages(
        await invoke("get_available_translation_languages"),
      );
    }

    void startup();
  }, []);

  const themeOptions = [
    {value: 0, label: _p("Theme", "Light")},
    {value: 1, label: _p("Theme", "Dark")},
    {value: 2, label: _p("Theme", "System")},
  ];

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
          <DialogTitle>{_g("Settings")}</DialogTitle>
        </DialogHeader>
        <VStack gap={4}>
          <Tabs
            defaultValue={page}
            onValueChange={(value) => setPage(value as SettingsDialogPage)}
          >
            <TabsList className="w-full">
              <TabsTrigger value={"general"}>
                <Settings2/>
                <span>{_g("General")}</span>
              </TabsTrigger>
              <TabsTrigger value={"advanced"}>
                <Wrench/>
                <span>{_g("Advanced")}</span>
              </TabsTrigger>
            </TabsList>
          </Tabs>
          <ScrollArea className="min-h-0 flex-1">
            {page === "general" && (
              <FieldGroup>
                <Field orientation="horizontal">
                  <FieldLabel>{_g("Theme")}</FieldLabel>
                  <Select
                    items={themeOptions}
                    value={configuration.theme}
                    onValueChange={(value) => {
                      if (value !== null) {
                        setConfiguration({...configuration, theme: value});
                      }
                    }}
                  >
                    <SelectTrigger>
                      <SelectValue/>
                    </SelectTrigger>
                    <SelectContent>
                      <SelectGroup>
                        {themeOptions.map((theme) => (
                          <SelectItem key={theme.value} value={theme.value}>
                            {theme.label}
                          </SelectItem>
                        ))}
                      </SelectGroup>
                    </SelectContent>
                  </Select>
                </Field>
                <Field orientation="horizontal">
                  <FieldLabel>{_g("Language")}</FieldLabel>
                  <Select
                    value={configuration.translationLanguage}
                    onValueChange={(value) => {
                      if (value !== null) {
                        setConfiguration({
                          ...configuration,
                          translationLanguage: value,
                        });
                      }
                    }}
                  >
                    <SelectTrigger>
                      <SelectValue/>
                    </SelectTrigger>
                    <SelectContent>
                      <SelectGroup>
                        {availableLanguages.map((language) => (
                          <SelectItem key={language} value={language}>
                            {language}
                          </SelectItem>
                        ))}
                      </SelectGroup>
                    </SelectContent>
                  </Select>
                </Field>
                <Field orientation="horizontal">
                  <FieldLabel>{_g("Automatically check for updates")}</FieldLabel>
                  <Switch
                    checked={configuration.automaticallyCheckForUpdates}
                    onCheckedChange={(checked) => {
                      setConfiguration({
                        ...configuration,
                        automaticallyCheckForUpdates: checked,
                      });
                    }}
                  />
                </Field>
              </FieldGroup>
            )}
            {page === "advanced" && (
              <VStack gap={4}>
                <FieldGroup>
                  <Field orientation="horizontal">
                    <FieldLabel>{_g("Allow preview (beta) updates")}</FieldLabel>
                    <Switch
                      checked={configuration.allowPreviewUpdates}
                      onCheckedChange={(checked) => {
                        setConfiguration({
                          ...configuration,
                          allowPreviewUpdates: checked,
                        });
                      }}
                    />
                  </Field>
                </FieldGroup>
              </VStack>
            )}
          </ScrollArea>
        </VStack>
        <DialogFooter>
          <DialogClose render={<Button variant="outline"/>}>
            {_p("Dialog", "Close")}
          </DialogClose>
        </DialogFooter>
      </DialogContent>
    </Dialog>
  );
}
