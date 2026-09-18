import {invoke} from "@tauri-apps/api/core";
import {LucideIcon, Settings2, Wrench} from "lucide-react";
import {useEffect, useState} from "react";

import {Button} from "@/components/ui/button.tsx";
import {
  Dialog,
  DialogClose,
  DialogContent,
  DialogFooter,
  DialogHeader,
  DialogTitle,
} from "@/components/ui/dialog.tsx";
import {
  Field,
  FieldContent,
  FieldGroup,
  FieldTitle,
} from "@/components/ui/field.tsx";
import {ScrollArea} from "@/components/ui/scroll-area.tsx";
import {
  Select,
  SelectContent,
  SelectItem,
  SelectTrigger,
  SelectValue,
} from "@/components/ui/select.tsx";
import {Separator} from "@/components/ui/separator.tsx";
import {Switch} from "@/components/ui/switch.tsx";
import {useConfiguration} from "@/lib/configuration-provider.tsx";
import {useDialog} from "@/lib/dialog-provider.tsx";
import {useTranslation} from "@/lib/translation-provider.tsx";

type SettingsDialogPage = "general" | "advanced";

interface SettingsDialogSection {
  page: SettingsDialogPage;
  title: string;
  icon: LucideIcon;
}

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

  const sections: SettingsDialogSection[] = [
    {
      page: "general",
      title: _g("General"),
      icon: Settings2,
    },
    {
      page: "advanced",
      title: _g("Advanced"),
      icon: Wrench,
    },
  ];

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
        <div className="flex h-72 min-h-0 gap-4">
          <div className="flex w-32 shrink-0 flex-col gap-1">
            {sections.map((section) => (
              <Button
                key={section.page}
                variant={page === section.page ? "secondary" : "ghost"}
                className="justify-start"
                onClick={() => {
                  setPage(section.page);
                }}
              >
                <section.icon />
                <span>{section.title}</span>
              </Button>
            ))}
          </div>
          <Separator orientation="vertical" />
          <ScrollArea className="min-h-0 flex-1">
            {page === "general" && (
              <FieldGroup className="p-1">
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
                    <SelectTrigger className="w-32">
                      <SelectValue />
                    </SelectTrigger>
                    <SelectContent>
                      {themeOptions.map((option) => (
                        <SelectItem key={option.value} value={option.value}>
                          {option.label}
                        </SelectItem>
                      ))}
                    </SelectContent>
                  </Select>
                </Field>
                <Field orientation="horizontal">
                  <FieldContent>
                    <FieldTitle>{_g("Translation Language")}</FieldTitle>
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
                    <SelectTrigger className="w-32">
                      <SelectValue />
                    </SelectTrigger>
                    <SelectContent>
                      {availableLanguages.map((language) => (
                        <SelectItem key={language} value={language}>
                          {language}
                        </SelectItem>
                      ))}
                    </SelectContent>
                  </Select>
                </Field>
              </FieldGroup>
            )}
            {page === "advanced" && (
              <FieldGroup className="p-1">
                <Field orientation="horizontal">
                  <FieldContent>
                    <FieldTitle>{_g("Allow Preview Updates")}</FieldTitle>
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
            )}
          </ScrollArea>
        </div>
        <DialogFooter>
          <DialogClose render={<Button variant="outline" />}>
            {_p("Dialog", "Close")}
          </DialogClose>
        </DialogFooter>
      </DialogContent>
    </Dialog>
  );
}
