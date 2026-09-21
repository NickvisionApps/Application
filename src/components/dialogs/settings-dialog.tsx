import {HStack, VStack} from "@/components/layout/stack.tsx";
import {
  Dialog,
  DialogContent,
  DialogHeader,
  DialogTitle,
} from "@/components/ui/dialog.tsx";
import {
  Field,
  FieldContent,
  FieldGroup,
  FieldLegend,
  FieldSeparator,
  FieldSet,
  FieldTitle,
} from "@/components/ui/field.tsx";
import {Input} from "@/components/ui/input.tsx";
import {ScrollArea} from "@/components/ui/scroll-area.tsx";
import {
  Select,
  SelectContent,
  SelectGroup,
  SelectItem,
  SelectTrigger,
  SelectValue,
} from "@/components/ui/select.tsx";
import {Switch} from "@/components/ui/switch.tsx";
import {Tabs, TabsList, TabsTrigger} from "@/components/ui/tabs.tsx";
import {useConfiguration} from "@/lib/contexts/configuration-context.ts";
import {useDialog} from "@/lib/contexts/dialog-context.ts";
import {useTranslation} from "@/lib/contexts/translation-context.ts";
import {invoke} from "@tauri-apps/api/core";
import {Settings2} from "lucide-react";
import {useEffect, useState} from "react";

type SettingsDialogPage = "general";

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
      <DialogContent className="flex max-h-[95vh] flex-col sm:max-w-lg">
        <DialogHeader>
          <DialogTitle>{_g("Settings")}</DialogTitle>
        </DialogHeader>
        <VStack gap={4} className="min-h-0 flex-1">
          <Input
            type="search"
            placeholder={_g("Search...")}
            className="w-full"
          />
          <HStack gap={6} className="min-h-0 flex-1">
            <Tabs
              orientation="vertical"
              defaultValue={page}
              onValueChange={(value) => setPage(value as SettingsDialogPage)}
              className="min-h-72 border-r border-border"
            >
              <TabsList variant="line">
                <TabsTrigger value={"general"}>
                  <Settings2 />
                  <span>{_g("General")}</span>
                </TabsTrigger>
              </TabsList>
            </Tabs>
            {page === "general" && (
              <ScrollArea className="flex-1">
                <FieldGroup className="overflow-hidden">
                  <FieldSet>
                    <FieldLegend>{_g("User Interface")}</FieldLegend>
                    <FieldGroup className="mt-2">
                      <Field orientation="horizontal">
                        <FieldContent>
                          <FieldTitle>{_g("Theme")}</FieldTitle>
                        </FieldContent>
                        <Select
                          items={themeOptions}
                          value={configuration.theme}
                          onValueChange={(value) => {
                            if (value !== null) {
                              setConfiguration({
                                ...configuration,
                                theme: value,
                              });
                            }
                          }}
                        >
                          <SelectTrigger>
                            <SelectValue />
                          </SelectTrigger>
                          <SelectContent>
                            <SelectGroup>
                              {themeOptions.map((theme) => (
                                <SelectItem
                                  key={theme.value}
                                  value={theme.value}
                                >
                                  {theme.label}
                                </SelectItem>
                              ))}
                            </SelectGroup>
                          </SelectContent>
                        </Select>
                      </Field>
                      <Field orientation="horizontal">
                        <FieldContent>
                          <FieldTitle>{_g("Language")}</FieldTitle>
                        </FieldContent>
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
                            <SelectValue />
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
                    </FieldGroup>
                  </FieldSet>
                  <FieldSeparator />
                  <FieldSet>
                    <FieldLegend>{_g("Updates")}</FieldLegend>
                    <FieldGroup className="mt-2">
                      <Field orientation="horizontal">
                        <FieldContent>
                          <FieldTitle>
                            {_g("Automatically check for updates")}
                          </FieldTitle>
                        </FieldContent>
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
                      <Field orientation="horizontal">
                        <FieldContent>
                          <FieldTitle>
                            {_g("Allow preview (beta) updates")}
                          </FieldTitle>
                        </FieldContent>
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
                  </FieldSet>
                </FieldGroup>
              </ScrollArea>
            )}
          </HStack>
        </VStack>
      </DialogContent>
    </Dialog>
  );
}
