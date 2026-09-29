import {HStack} from "@/components/layout/stack.tsx";
import {NavigationView} from "@/components/navigation-view.tsx";
import {FolderPage} from "@/components/pages/folder-page.tsx";
import {HomePage} from "@/components/pages/home-page.tsx";
import {SidebarTrigger, useSidebar} from "@/components/ui/sidebar.tsx";
import {Toaster} from "@/components/ui/toast.tsx";
import {
  Tooltip,
  TooltipContent,
  TooltipTrigger,
} from "@/components/ui/tooltip.tsx";
import {WindowControls} from "@/components/window-controls.tsx";
import {useKeyboardShortcut} from "@/hooks/use-keyboard-shortcut.ts";
import {useDialog} from "@/lib/contexts/dialog-context.ts";
import {useFolderView} from "@/lib/contexts/folder-view-context.ts";
import {useNavigation} from "@/lib/contexts/navigation-context.ts";
import {useTitlebar} from "@/lib/contexts/titlebar-context.ts";
import {useTranslation} from "@/lib/contexts/translation-context.ts";
import {invoke} from "@tauri-apps/api/core";
import {getCurrentWindow} from "@tauri-apps/api/window";
import {platform} from "@tauri-apps/plugin-os";
import {saveWindowState, StateFlags} from "@tauri-apps/plugin-window-state";
import {useEffect} from "react";

export function Window() {
  const {_g} = useTranslation();
  const {page} = useNavigation();
  const {openDialog} = useDialog();
  const {open: sidebarOpen, isMobile, openMobile} = useSidebar();
  const {folderView, openFolder, closeFolder} = useFolderView();
  const {content, side} = useTitlebar();

  useEffect(() => {
    const window = getCurrentWindow();
    let closeFn: () => void;

    async function startup() {
      await invoke("show_main_window");
      closeFn = await window.onCloseRequested(async (event) => {
        await saveWindowState(StateFlags.ALL);
        if (!(await invoke<boolean>("can_window_close"))) {
          openDialog("close");
          event.preventDefault();
        }
      });
    }

    document.documentElement.dataset.platform = platform();
    void startup();

    return () => {
      if (closeFn) {
        closeFn();
      }
    };
  }, []);

  useKeyboardShortcut(",", () => {
    openDialog("settings");
  });

  useKeyboardShortcut(
    "d",
    () => {
      openDialog("debugging");
    },
    {shift: true},
  );

  useKeyboardShortcut("o", () => {
    void openFolder();
  });

  useKeyboardShortcut(
    "w",
    () => {
      void closeFolder();
    },
    {shift: true, enabled: Boolean(folderView.path)},
  );

  return (
    <>
      <div className="titlebar-drag-region" data-tauri-drag-region="deep">
        {(platform() === "windows" || platform() === "linux") &&
          side === "left" && <WindowControls />}
        <Tooltip>
          <TooltipTrigger
            render={
              <SidebarTrigger
                className={platform() === "macos" ? "mt-2" : ""}
              />
            }
          />
          <TooltipContent side="right">
            {(isMobile ? openMobile : sidebarOpen)
              ? _g("Hide Sidebar")
              : _g("Show Sidebar")}
          </TooltipContent>
        </Tooltip>
        {content}
        {(platform() === "windows" || platform() === "linux") &&
          side !== "left" && <WindowControls />}
      </div>
      <HStack className="h-screen w-full overflow-hidden">
        <NavigationView />
        <main className="min-w-0 flex-1 overflow-hidden pt-(--titlebar-height)">
          {page === "home" && <HomePage />}
          {page === "folder" && <FolderPage />}
          <Toaster />
        </main>
      </HStack>
    </>
  );
}
