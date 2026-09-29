import {Grid} from "@/components/layout/grid.tsx";
import {HStack, VStack} from "@/components/layout/stack.tsx";
import {Button} from "@/components/ui/button.tsx";
import {
  Empty,
  EmptyDescription,
  EmptyHeader,
  EmptyMedia,
  EmptyTitle,
} from "@/components/ui/empty.tsx";
import {Kbd, KbdGroup} from "@/components/ui/kbd.tsx";
import {
  ResizableHandle,
  ResizablePanel,
  ResizablePanelGroup,
} from "@/components/ui/resizable.tsx";
import {ScrollArea} from "@/components/ui/scroll-area.tsx";
import {ToggleGroup, ToggleGroupItem} from "@/components/ui/toggle-group.tsx";
import {
  Tooltip,
  TooltipContent,
  TooltipTrigger,
} from "@/components/ui/tooltip.tsx";
import {useTitlebarControls} from "@/hooks/titlebar.tsx";
import {useFolderView} from "@/lib/contexts/folder-view-context.ts";
import {useTranslation} from "@/lib/contexts/translation-context.ts";
import {convertFileSrc} from "@tauri-apps/api/core";
import {platform} from "@tauri-apps/plugin-os";
import {cn} from "cn";
import {
  FileIcon,
  FolderOpenIcon,
  LayoutGridIcon,
  ListIcon,
  XIcon,
} from "lucide-react";
import {useEffect, useMemo, useState} from "react";

const IMAGE_EXTENSIONS = new Set([
  "png",
  "jpg",
  "jpeg",
  "gif",
  "webp",
  "bmp",
  "avif",
  "ico",
]);

interface FolderFile {
  path: string;
  name: string;
}

export function FolderPage() {
  const {_g} = useTranslation();
  const {folderView, closeFolder, openFolder} = useFolderView();
  const [selectedFile, setSelectedFile] = useState<FolderFile | null>(null);
  const [imageLoadFailed, setImageLoadFailed] = useState(false);
  const [viewMode, setViewMode] = useState<"list" | "grid">("list");

  useEffect(() => {
    setSelectedFile(null);
  }, [folderView.path]);

  useEffect(() => {
    setImageLoadFailed(false);
  }, [selectedFile]);

  const files = useMemo(() => {
    return folderView.files.map((filePath): FolderFile => ({
      path: filePath,
      name: filePath.split(/[/\\]/).filter(Boolean).pop() ?? filePath,
    }));
  }, [folderView.files]);

  const selectedExtension = selectedFile
    ? selectedFile.path
        .slice(selectedFile.path.lastIndexOf(".") + 1)
        .toLowerCase()
    : "";
  const canPreviewImage =
    selectedFile !== null &&
    IMAGE_EXTENSIONS.has(selectedExtension) &&
    !imageLoadFailed;

  useTitlebarControls(
    <>
      <ToggleGroup
        variant="outline"
        value={[viewMode]}
        onValueChange={(value) => {
          if (value.length > 0) {
            setViewMode(value[0] as "list" | "grid");
          }
        }}
      >
        <Tooltip>
          <TooltipTrigger
            render={
              <ToggleGroupItem value="list" aria-label={_g("List View")}>
                <ListIcon />
              </ToggleGroupItem>
            }
          />
          <TooltipContent>{_g("List View")}</TooltipContent>
        </Tooltip>
        <Tooltip>
          <TooltipTrigger
            render={
              <ToggleGroupItem value="grid" aria-label={_g("Grid View")}>
                <LayoutGridIcon />
              </ToggleGroupItem>
            }
          />
          <TooltipContent>{_g("Grid View")}</TooltipContent>
        </Tooltip>
      </ToggleGroup>
      <Tooltip>
        <TooltipTrigger
          render={
            <Button
              variant="outline"
              size="icon"
              onClick={() => {
                void openFolder();
              }}
            >
              <FolderOpenIcon />
            </Button>
          }
        />
        <TooltipContent>
          {_g("Open Folder")}
          <KbdGroup>
            {platform() === "macos" && <Kbd>⌘</Kbd>}
            {platform() !== "macos" && <Kbd>Ctrl</Kbd>}
            <Kbd>O</Kbd>
          </KbdGroup>
        </TooltipContent>
      </Tooltip>
      <Tooltip>
        <TooltipTrigger
          render={
            <Button
              variant="outline"
              size="icon"
              onClick={() => {
                void closeFolder();
              }}
            >
              <XIcon />
            </Button>
          }
        />
        <TooltipContent>
          {_g("Close Folder")}
          <KbdGroup>
            {platform() === "macos" && <Kbd>⌘</Kbd>}
            {platform() !== "macos" && <Kbd>Ctrl</Kbd>}
            <Kbd>Shift</Kbd>
            <span>+</span>
            <Kbd>W</Kbd>
          </KbdGroup>
        </TooltipContent>
      </Tooltip>
    </>,
  );

  return (
    <VStack className="h-full w-full">
      <ResizablePanelGroup className="min-h-0 flex-1">
        <ResizablePanel defaultSize="35" minSize="20">
          <ScrollArea className="h-full">
            {viewMode === "list" ? (
              <VStack gap={0.5} className="p-2">
                {files.length > 0 ? (
                  files.map((file) => (
                    <HStack
                      key={file.path}
                      render={<Button variant="ghost" />}
                      align="center"
                      justify="start"
                      gap={1.5}
                      onClick={() => {
                        setSelectedFile(file);
                      }}
                      className={cn(
                        "w-full px-2 py-1 text-left text-sm",
                        selectedFile?.path === file.path &&
                          "bg-muted font-medium",
                      )}
                    >
                      <FileIcon className="size-4 shrink-0 text-muted-foreground" />
                      <span className="truncate">{file.name}</span>
                    </HStack>
                  ))
                ) : (
                  <p className="p-2 text-sm text-muted-foreground">
                    {_g("This folder is empty.")}
                  </p>
                )}
              </VStack>
            ) : (
              <Grid columns={3} gap={2} className="p-2">
                {files.length > 0 ? (
                  files.map((file) => (
                    <VStack
                      key={file.path}
                      render={<Button variant="ghost" />}
                      align="center"
                      justify="center"
                      gap={1}
                      onClick={() => {
                        setSelectedFile(file);
                      }}
                      className={cn(
                        "h-auto w-full p-3 text-center text-sm",
                        selectedFile?.path === file.path &&
                          "bg-muted font-medium",
                      )}
                    >
                      <FileIcon className="size-10 shrink-0 text-muted-foreground" />
                      <span className="line-clamp-2 w-full break-words text-xs">
                        {file.name}
                      </span>
                    </VStack>
                  ))
                ) : (
                  <p className="col-span-full p-2 text-sm text-muted-foreground">
                    {_g("This folder is empty.")}
                  </p>
                )}
              </Grid>
            )}
          </ScrollArea>
        </ResizablePanel>
        <ResizableHandle withHandle />
        <ResizablePanel defaultSize="65" minSize="30">
          <HStack align="center" justify="center" className="h-full w-full p-4">
            {selectedFile === null ? (
              <Empty>
                <EmptyHeader>
                  <EmptyTitle>{_g("No File Selected")}</EmptyTitle>
                  <EmptyDescription>
                    {_g("Select a file to preview it here.")}
                  </EmptyDescription>
                </EmptyHeader>
              </Empty>
            ) : canPreviewImage ? (
              <img
                src={convertFileSrc(selectedFile.path)}
                alt={selectedFile.path}
                onError={() => {
                  setImageLoadFailed(true);
                }}
                className="max-h-full max-w-full rounded-lg object-contain"
              />
            ) : (
              <Empty>
                <EmptyHeader>
                  <EmptyMedia variant="icon">
                    <FileIcon />
                  </EmptyMedia>
                  <EmptyTitle>{_g("No Preview Available")}</EmptyTitle>
                  <EmptyDescription>{selectedFile.name}</EmptyDescription>
                </EmptyHeader>
              </Empty>
            )}
          </HStack>
        </ResizablePanel>
      </ResizablePanelGroup>
    </VStack>
  );
}
