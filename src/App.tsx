import {SidebarProvider} from "@/components/ui/sidebar.tsx";
import {TooltipProvider} from "@/components/ui/tooltip.tsx";
import {Window} from "@/components/window.tsx";
import {ConfigurationProvider} from "@/lib/providers/configuration-provider.tsx";
import {DialogProvider} from "@/lib/providers/dialog-provider.tsx";
import {FolderViewProvider} from "@/lib/providers/folder-view-provider.tsx";
import {NavigationProvider} from "@/lib/providers/navigation-provider.tsx";
import {ProductInfoProvider} from "@/lib/providers/product-info-provider.tsx";
import {TitlebarProvider} from "@/lib/providers/titlebar-provider.tsx";
import {TranslationProvider} from "@/lib/providers/translation-provider.tsx";

export function App() {
  return (
    <ProductInfoProvider>
      <ConfigurationProvider>
        <TranslationProvider>
          <TitlebarProvider>
            <DialogProvider>
              <NavigationProvider>
                <TooltipProvider>
                  <SidebarProvider>
                    <FolderViewProvider>
                      <Window />
                    </FolderViewProvider>
                  </SidebarProvider>
                </TooltipProvider>
              </NavigationProvider>
            </DialogProvider>
          </TitlebarProvider>
        </TranslationProvider>
      </ConfigurationProvider>
    </ProductInfoProvider>
  );
}
