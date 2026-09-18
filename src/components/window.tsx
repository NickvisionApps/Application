import {NavigationView} from "@/components/navigation-view.tsx";
import {FolderPage} from "@/components/pages/folder-page.tsx";
import {HomePage} from "@/components/pages/home-page.tsx";
import {TitlebarControlEscape} from "@/components/titlebar-control-escape.tsx";
import {SidebarTrigger, useSidebar} from "@/components/ui/sidebar.tsx";
import {Toaster} from "@/components/ui/toast.tsx";
import {
  Tooltip,
  TooltipContent,
  TooltipTrigger,
} from "@/components/ui/tooltip.tsx";
import {useNavigation} from "@/lib/navigation-provider.tsx";
import {useTranslation} from "@/lib/translation-provider.tsx";
import {invoke} from "@tauri-apps/api/core";
import {platform} from "@tauri-apps/plugin-os";
import {useEffect} from "react";

export function Window() {
  const {_g} = useTranslation();
  const {page} = useNavigation();
  const {open, isMobile, openMobile} = useSidebar();

  useEffect(() => {
    async function startup() {
      await invoke("show_main_window");
    }

    void startup();
  }, []);

  return (
    <div className="flex h-screen w-full">
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
    </div>
  );
}
