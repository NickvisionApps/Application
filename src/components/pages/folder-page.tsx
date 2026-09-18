import {TitlebarControlEscape} from "@/components/titlebar-control-escape.tsx";
import {Button} from "@/components/ui/button.tsx";
import {Empty, EmptyDescription, EmptyHeader, EmptyMedia, EmptyTitle} from "@/components/ui/empty.tsx";
import {ResizableHandle, ResizablePanel, ResizablePanelGroup} from "@/components/ui/resizable.tsx";
import {ScrollArea} from "@/components/ui/scroll-area.tsx";
import {useSidebar} from "@/components/ui/sidebar.tsx";
import {Tooltip, TooltipContent, TooltipTrigger} from "@/components/ui/tooltip.tsx";
import {useFolderView} from "@/lib/folder-view-provider.tsx";
import {useNavigation} from "@/lib/navigation-provider.tsx";
import {useTranslation} from "@/lib/translation-provider.tsx";
import {convertFileSrc} from "@tauri-apps/api/core";
import {platform} from "@tauri-apps/plugin-os";
import {cn} from "cn";
import {FileIcon, FolderOpenIcon, XIcon} from "lucide-react";
import {useEffect, useMemo, useState} from "react";

const IMAGE_EXTENSIONS = new Set(["png", "jpg", "jpeg", "gif", "webp", "bmp", "avif", "ico"]);

interface FolderFile {
  path: string,
  name: string
}

export function FolderPage() {
  const {_g} = useTranslation();
  const {setPage} = useNavigation();
  const {open, isMobile} = useSidebar();
  const {folderView, closeFolder, openFolder} = useFolderView();
  const [selectedFile, setSelectedFile] = useState<FolderFile | null>(null);
  const [imageLoadFailed, setImageLoadFailed] = useState(false);

  useEffect(() => {
    setSelectedFile(null);
  }, [folderView.path]);

  useEffect(() => {
    setImageLoadFailed(false);
  }, [selectedFile]);

  const files = useMemo(() => {
    return folderView.files.map((filePath): FolderFile => ({
      path: filePath,
      name: filePath.split(/[/\\]/).filter(Boolean).pop() ?? filePath
    }));
  }, [folderView.files]);

  const selectedExtension = selectedFile ? selectedFile.path.slice(selectedFile.path.lastIndexOf(".") + 1).toLowerCase() : "";
  const canPreviewImage = selectedFile !== null && IMAGE_EXTENSIONS.has(selectedExtension) && !imageLoadFailed;

  return (
    <div className="flex h-full w-full flex-col">
      <div className="flex items-center gap-2 border-b p-2">
        <p
          className={((!open || isMobile) && platform() === "macos") ? "ml-30 min-w-0 flex-1 truncate text-sm text-muted-foreground" : "min-w-0 flex-1 truncate text-sm text-muted-foreground"}
          title={folderView.path}>{folderView.path}</p>
        <TitlebarControlEscape>
          <Tooltip>
            <TooltipTrigger render={<Button variant="outline" size="icon" onClick={async () => {
              await openFolder();
            }}>
              <FolderOpenIcon/>
            </Button>}/>
            <TooltipContent>{_g("Open Folder")}</TooltipContent>
          </Tooltip>
        </TitlebarControlEscape>
        <TitlebarControlEscape>
          <Tooltip>
            <TooltipTrigger render={<Button variant="outline" onClick={async () => {
              await closeFolder();
              setPage("home");
            }}>
              <XIcon/>
              {_g("Close")}
            </Button>}/>
            <TooltipContent>{_g("Close Folder")}</TooltipContent>
          </Tooltip>
        </TitlebarControlEscape>
      </div>
      <ResizablePanelGroup className="min-h-0 flex-1">
        <ResizablePanel defaultSize="35" minSize="20">
          <ScrollArea className="h-full">
            <div className="flex flex-col gap-0.5 p-2">
              {files.length > 0 ? (
                files.map((file) => (
                  <button
                    key={file.path}
                    type="button"
                    onClick={() => setSelectedFile(file)}
                    className={cn(
                      "flex w-full items-center gap-1.5 rounded-md px-2 py-1 text-left text-sm hover:bg-muted",
                      selectedFile?.path === file.path && "bg-muted font-medium"
                    )}
                  >
                    <FileIcon className="size-4 shrink-0 text-muted-foreground"/>
                    <span className="truncate">{file.name}</span>
                  </button>
                ))
              ) : (
                <p className="p-2 text-sm text-muted-foreground">{_g("This folder is empty.")}</p>
              )}
            </div>
          </ScrollArea>
        </ResizablePanel>
        <ResizableHandle withHandle/>
        <ResizablePanel defaultSize="65" minSize="30">
          <div className="flex h-full w-full items-center justify-center p-4">
            {selectedFile === null ? (
              <Empty>
                <EmptyHeader>
                  <EmptyTitle>{_g("No File Selected")}</EmptyTitle>
                  <EmptyDescription>{_g("Select a file to preview it here.")}</EmptyDescription>
                </EmptyHeader>
              </Empty>
            ) : canPreviewImage ? (
              <img
                src={convertFileSrc(selectedFile.path)}
                alt={selectedFile.path}
                onError={() => setImageLoadFailed(true)}
                className="max-h-full max-w-full rounded-lg object-contain"
              />
            ) : (
              <Empty>
                <EmptyHeader>
                  <EmptyMedia variant="icon">
                    <FileIcon/>
                  </EmptyMedia>
                  <EmptyTitle>{_g("No Preview Available")}</EmptyTitle>
                  <EmptyDescription>{selectedFile.name}</EmptyDescription>
                </EmptyHeader>
              </Empty>
            )}
          </div>
        </ResizablePanel>
      </ResizablePanelGroup>
    </div>
  )
}