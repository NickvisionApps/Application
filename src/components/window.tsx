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
import {useConfiguration} from "@/lib/contexts/configuration-context.ts";
import {useDialog} from "@/lib/contexts/dialog-context.ts";
import {useFolderView} from "@/lib/contexts/folder-view-context.ts";
import {useNavigation} from "@/lib/contexts/navigation-context.ts";
import {useTitlebar} from "@/lib/contexts/titlebar-context.ts";
import {useTranslation} from "@/lib/contexts/translation-context.ts";
import {invoke} from "@tauri-apps/api/core";
import {getCurrentWindow} from "@tauri-apps/api/window";
import {platform} from "@tauri-apps/plugin-os";
import {useEffect, useRef} from "react";

export function Window() {
  const {configuration, setConfiguration} = useConfiguration();
  const {_g} = useTranslation();
  const {page, setPage} = useNavigation();
  const {openDialog} = useDialog();
  const {open: sidebarOpen, isMobile, openMobile} = useSidebar();
  const {folderView, openFolder, closeFolder} = useFolderView();
  const {content} = useTitlebar();
  const configurationRef = useRef(configuration);

  configurationRef.current = configuration;

  useEffect(() => {
    const window = getCurrentWindow();
    let closeFn: () => void;
    let resizeFn: () => void;
    let moveFn: () => void;

    async function saveWindowGeometry() {
      const window = getCurrentWindow();
      if ((await window.isMaximized()) || (await window.isFullscreen())) {
        setConfiguration({
          ...configurationRef.current,
          windowGeometry: {
            x: 10,
            y: 10,
            width: 800,
            height: 600,
            isMaximized: true,
          },
        });
      } else {
        const scale = await window.scaleFactor();
        const size = (await window.innerSize()).toLogical(scale);
        const position = (await window.outerPosition()).toLogical(scale);
        setConfiguration({
          ...configurationRef.current,
          windowGeometry: {
            x: Math.floor(position.x),
            y: Math.floor(position.y),
            width: Math.floor(size.width),
            height: Math.floor(size.height),
            isMaximized: false,
          },
        });
      }
    }

    async function startup() {
      await invoke("show_main_window");
      closeFn = await window.onCloseRequested(async (event) => {
        const canClose = await invoke<boolean>("can_window_close");
        if (!canClose) {
          openDialog("close");
          event.preventDefault();
        } else {
          if (platform() === "macos") {
            await window.hide();
            event.preventDefault();
          }
        }
      });
      resizeFn = await window.onResized(async () => await saveWindowGeometry());
      moveFn = await window.onMoved(async () => await saveWindowGeometry());
    }

    void startup();

    return () => {
      if (closeFn) {
        closeFn();
      }
      if (resizeFn) {
        resizeFn();
      }
      if (moveFn) {
        moveFn();
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
            {(isMobile ? openMobile : sidebarOpen)
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
