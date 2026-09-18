import {SidebarProvider} from "@/components/ui/sidebar.tsx";
import {TooltipProvider} from "@/components/ui/tooltip.tsx";
import {Window} from "@/components/window.tsx";
import {ConfigurationProvider} from "@/lib/configuration-provider.tsx";
import {DialogProvider} from "@/lib/dialog-provider.tsx";
import {FolderViewProvider} from "@/lib/folder-view-provider.tsx";
import {NavigationProvider} from "@/lib/navigation-provider.tsx";
import {TranslationProvider} from "@/lib/translation-provider.tsx";

export function App() {
  return (
    <ConfigurationProvider>
      <TranslationProvider>
        <DialogProvider>
          <NavigationProvider>
            <TooltipProvider>
              <SidebarProvider>
                <FolderViewProvider>
                  <Window/>
                </FolderViewProvider>
              </SidebarProvider>
            </TooltipProvider>
          </NavigationProvider>
        </DialogProvider>
      </TranslationProvider>
    </ConfigurationProvider>
  )
}
