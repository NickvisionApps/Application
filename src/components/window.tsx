import {HStack} from "@/components/layout/stack.tsx";
import {NavigationView} from "@/components/navigation-view.tsx";
import {FolderPage} from "@/components/pages/folder-page.tsx";
import {HomePage} from "@/components/pages/home-page.tsx";
import {TitlebarControlEscape} from "@/components/titlebar-control-escape.tsx";
import {SidebarTrigger, useSidebar} from "@/components/ui/sidebar.tsx";
import {Toaster, toast} from "@/components/ui/toast.tsx";
import {
  Tooltip,
  TooltipContent,
  TooltipTrigger,
} from "@/components/ui/tooltip.tsx";
import {useFolderView} from "@/lib/folder-view-provider.tsx";
import {useKeyboardShortcut} from "@/lib/keyboard-shortcut-provider.tsx";
import {useNavigation} from "@/lib/navigation-provider.tsx";
import {useTranslation} from "@/lib/translation-provider.tsx";
import {invoke} from "@tauri-apps/api/core";
import {platform} from "@tauri-apps/plugin-os";
import {useEffect} from "react";

export function Window() {
  const {_g} = useTranslation();
  const {page, setPage} = useNavigation();
  const {open, isMobile, openMobile} = useSidebar();
  const {folderView, openFolder, closeFolder} = useFolderView();

  useEffect(() => {
    async function startup() {
      await invoke("show_main_window");
    }

    void startup();
  }, []);

  useKeyboardShortcut("o", () => {
    async function handleOpenFolder() {
      if (await openFolder()) {
        setPage("folder");
      }
    }

    void handleOpenFolder();
  });

  useKeyboardShortcut(
    "w",
    () => {
      async function handleCloseFolder() {
        await closeFolder();
        toast.add({title: _g("Folder closed")});
        setPage("home");
      }

      void handleCloseFolder();
    },
    {shift: true, enabled: Boolean(folderView.path)},
  );

  return (
    <HStack className="h-screen w-full">
      <NavigationView />
      {(!open || isMobile) && (
        <TitlebarControlEscape renderAbsolute>
          <Tooltip>
            <TooltipTrigger
              render={
                <SidebarTrigger
                  className={platform() === "macos" ? "ml-22 mt-2.5" : ""}
                />
              }
            />
            <TooltipContent side="right">
              {openMobile ? _g("Hide Sidebar") : _g("Show Sidebar")}
            </TooltipContent>
          </Tooltip>
        </TitlebarControlEscape>
      )}
      <main className="flex-1">
        {page === "home" && <HomePage />}
        {page === "folder" && <FolderPage />}
        <Toaster />
      </main>
    </HStack>
  );
}
