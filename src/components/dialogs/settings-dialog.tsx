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
import {CloudDownload, Settings2} from "lucide-react";
import {useEffect, useState} from "react";

const SETTINGS_PAGES = ["general", "updates"] as const;

type SettingsDialogPage = (typeof SETTINGS_PAGES)[number];

export function SettingsDialog() {
  const {closeDialog} = useDialog();
  const {configuration, setConfiguration} = useConfiguration();
  const {_g, _p, _f} = useTranslation();
  const [availableLanguages, setAvailableLanguages] = useState<string[]>([]);
  const [page, setPage] = useState<SettingsDialogPage>("general");
  const [search, setSearch] = useState("");

  const themeOptions = [
    {value: 0, label: _p("Theme", "Light")},
    {value: 1, label: _p("Theme", "Dark")},
    {value: 2, label: _p("Theme", "System")},
  ];
  const query = search.trim().toLowerCase();
  const isSearching = query.length > 0;
  const matchingPages = new Set(
    (
      [
        {page: "general", label: _g("Theme")},
        {page: "general", label: _g("Language")},
        {page: "updates", label: _g("Automatically check for updates")},
        {page: "updates", label: _g("Allow preview (beta) updates")},
      ] satisfies {page: SettingsDialogPage; label: string}[]
    )
      .filter((field) => field.label.toLowerCase().includes(query))
      .map((field) => field.page),
  );

  useEffect(() => {
    async function startup() {
      setAvailableLanguages(
        await invoke("get_available_translation_languages"),
      );
    }

    void startup();
  }, []);

  useEffect(() => {
    if (isSearching && !matchingPages.has(page)) {
      const nextPage = SETTINGS_PAGES.find((candidate) =>
        matchingPages.has(candidate),
      );
      if (nextPage) {
        setPage(nextPage);
      }
    }
  }, [page, isSearching, matchingPages]);

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
            value={search}
            onChange={(event) => setSearch(event.target.value)}
          />
          <HStack gap={6} className="min-h-0 flex-1">
            <Tabs
              orientation="vertical"
              value={page}
              onValueChange={(value) => setPage(value as SettingsDialogPage)}
              className={
                !isSearching || matchingPages.has(page)
                  ? "min-h-72 border-r border-border"
                  : "min-h-72"
              }
            >
              <TabsList variant="line">
                {(!isSearching || matchingPages.has("general")) && (
                  <TabsTrigger value={"general"}>
                    <Settings2 />
                    <span>{_g("General")}</span>
                  </TabsTrigger>
                )}
                {(!isSearching || matchingPages.has("updates")) && (
                  <TabsTrigger value={"updates"}>
                    <CloudDownload />
                    <span>{_g("Updates")}</span>
                  </TabsTrigger>
                )}
              </TabsList>
            </Tabs>
            {page === "general" &&
              (!isSearching || matchingPages.has("general")) && (
                <ScrollArea className="flex-1">
                  <FieldGroup className="overflow-hidden">
                    <FieldSet>
                      <FieldGroup className="mt-2">
                        {(!isSearching ||
                          _g("Theme").toLowerCase().includes(query)) && (
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
                        )}
                        {(!isSearching ||
                          _g("Language").toLowerCase().includes(query)) && (
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
                        )}
                      </FieldGroup>
                    </FieldSet>
                  </FieldGroup>
                </ScrollArea>
              )}
            {page === "updates" &&
              (!isSearching || matchingPages.has("updates")) && (
                <ScrollArea className="flex-1">
                  <FieldGroup className="overflow-hidden">
                    <FieldSet>
                      <FieldGroup className="mt-2">
                        {(!isSearching ||
                          _g("Automatically check for updates")
                            .toLowerCase()
                            .includes(query)) && (
                          <Field orientation="horizontal">
                            <FieldContent>
                              <FieldTitle>
                                {_g("Automatically check for updates")}
                              </FieldTitle>
                            </FieldContent>
                            <Switch
                              checked={
                                configuration.automaticallyCheckForUpdates
                              }
                              onCheckedChange={(checked) => {
                                setConfiguration({
                                  ...configuration,
                                  automaticallyCheckForUpdates: checked,
                                });
                              }}
                            />
                          </Field>
                        )}
                        {(!isSearching ||
                          _g("Allow preview (beta) updates")
                            .toLowerCase()
                            .includes(query)) && (
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
                        )}
                      </FieldGroup>
                    </FieldSet>
                  </FieldGroup>
                </ScrollArea>
              )}
            {isSearching && !matchingPages.has(page) && (
              <ScrollArea className="flex-1">
                <p className="py-8 text-center text-sm text-muted-foreground">
                  {_f("No results found for “{0}”", [search])}
                </p>
              </ScrollArea>
            )}
          </HStack>
        </VStack>
      </DialogContent>
    </Dialog>
  );
}
