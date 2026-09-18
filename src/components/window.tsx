import {invoke} from "@tauri-apps/api/core";
import {platform} from "@tauri-apps/plugin-os";
import {useEffect} from "react";

import {NavigationView} from "@/components/navigation-view.tsx";
import {FolderPage} from "@/components/pages/folder-page.tsx";
import {HomePage} from "@/components/pages/home-page.tsx";
import {TitlebarControlEscape} from "@/components/titlebar-control-escape.tsx";
import {
  SidebarInset,
  SidebarTrigger,
  useSidebar,
} from "@/components/ui/sidebar.tsx";
import {Toaster} from "@/components/ui/toast.tsx";
import {
  Tooltip,
  TooltipContent,
  TooltipTrigger,
} from "@/components/ui/tooltip.tsx";
import {useNavigation} from "@/lib/navigation-provider.tsx";
import {useTranslation} from "@/lib/translation-provider.tsx";

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
    <div className="flex h-screen w-full bg-transparent">
      <NavigationView />
      <SidebarInset className="bg-background/60 backdrop-blur-md">
        {(!open || isMobile) && (
          <TitlebarControlEscape renderAbsolute>
            <Tooltip>
              <TooltipTrigger
                render={
                  <SidebarTrigger
                    className={
                      platform() === "macos"
                        ? isMobile
                          ? "ml-22 mt-2.5"
                          : "ml-20 mt-0.75"
                        : ""
                    }
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
        </main>
        <Toaster />
      </SidebarInset>
    </div>
  );
}
