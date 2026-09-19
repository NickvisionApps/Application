import {HStack} from "@/components/layout/stack.tsx";
import {NavigationView} from "@/components/navigation-view.tsx";
import {FolderPage} from "@/components/pages/folder-page.tsx";
import {HomePage} from "@/components/pages/home-page.tsx";
import {SidebarTrigger, useSidebar} from "@/components/ui/sidebar.tsx";
import {toast, Toaster} from "@/components/ui/toast.tsx";
import {
  Tooltip,
  TooltipContent,
  TooltipTrigger,
} from "@/components/ui/tooltip.tsx";
import {useKeyboardShortcut} from "@/hooks/use-keyboard-shortcut.ts";
import {useFolderView} from "@/lib/folder-view-provider.tsx";
import {useNavigation} from "@/lib/navigation-provider.tsx";
import {useTitlebar} from "@/lib/titlebar-provider.tsx";
import {useTranslation} from "@/lib/translation-provider.tsx";
import {invoke} from "@tauri-apps/api/core";
import {useEffect} from "react";

export function Window() {
  const {_g} = useTranslation();
  const {page, setPage} = useNavigation();
  const {open, isMobile, openMobile} = useSidebar();
  const {folderView, openFolder, closeFolder} = useFolderView();
  const {content} = useTitlebar();

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
        toast.add({
          type: "info",
          title: _g("Folder closed"),
        });
        setPage("home");
      }

      void handleCloseFolder();
    },
    {shift: true, enabled: Boolean(folderView.path)},
  );

  return (
    <>
      <div className="titlebar-drag-region" data-tauri-drag-region="deep">
        <Tooltip>
          <TooltipTrigger render={<SidebarTrigger />} />
          <TooltipContent side="right">
            {(isMobile ? openMobile : open)
              ? _g("Hide Sidebar")
              : _g("Show Sidebar")}
          </TooltipContent>
        </Tooltip>
        {content}
      </div>
      <HStack className="h-screen w-full overflow-hidden">
        <NavigationView />
        <main className="min-w-0 flex-1 overflow-hidden pt-(--tauri-plugin-decoration-titlebar-height)">
          {page === "home" && <HomePage />}
          {page === "folder" && <FolderPage />}
          <Toaster />
        </main>
      </HStack>
    </>
  );
}
