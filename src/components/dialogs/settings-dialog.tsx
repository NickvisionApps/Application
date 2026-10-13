import {
  Dialog,
  DialogContent,
  DialogDescription,
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
import {
  Sidebar,
  SidebarContent,
  SidebarGroup,
  SidebarGroupContent,
  SidebarMenu,
  SidebarMenuButton,
  SidebarMenuItem,
  SidebarProvider,
} from "@/components/ui/sidebar.tsx";
import {Switch} from "@/components/ui/switch.tsx";
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
      <DialogContent className="gap-0 overflow-hidden p-0 sm:max-w-3xl">
        <DialogDescription className="sr-only">
          {_g("Settings")}
        </DialogDescription>
        <SidebarProvider className="h-[480px] min-h-0 items-start">
          <Sidebar collapsible="none">
            <SidebarContent>
              <SidebarGroup>
                <SidebarGroupContent>
                  <SidebarMenu>
                    {(!isSearching || matchingPages.has("general")) && (
                      <SidebarMenuItem>
                        <SidebarMenuButton
                          isActive={page === "general"}
                          onClick={() => setPage("general")}
                        >
                          <Settings2 />
                          <span>{_g("General")}</span>
                        </SidebarMenuButton>
                      </SidebarMenuItem>
                    )}
                    {(!isSearching || matchingPages.has("updates")) && (
                      <SidebarMenuItem>
                        <SidebarMenuButton
                          isActive={page === "updates"}
                          onClick={() => setPage("updates")}
                        >
                          <CloudDownload />
                          <span>{_g("Updates")}</span>
                        </SidebarMenuButton>
                      </SidebarMenuItem>
                    )}
                  </SidebarMenu>
                </SidebarGroupContent>
              </SidebarGroup>
            </SidebarContent>
          </Sidebar>
          <main className="flex h-full min-w-0 flex-1 flex-col overflow-hidden">
            <header className="flex h-14 shrink-0 items-center justify-between gap-4 pr-12 pl-4">
              <DialogTitle>{_g("Settings")}</DialogTitle>
              <Input
                type="search"
                placeholder={_g("Search...")}
                className="w-48"
                value={search}
                onChange={(event) => setSearch(event.target.value)}
              />
            </header>
            {page === "general" &&
              (!isSearching || matchingPages.has("general")) && (
                <ScrollArea className="min-h-0 flex-1 px-4">
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
                <ScrollArea className="min-h-0 flex-1 px-4">
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
              <ScrollArea className="min-h-0 flex-1 px-4">
                <p className="py-8 text-center text-sm text-muted-foreground">
                  {_f("No results found for “{0}”", [search])}
                </p>
              </ScrollArea>
            )}
          </main>
        </SidebarProvider>
      </DialogContent>
    </Dialog>
  );
}
