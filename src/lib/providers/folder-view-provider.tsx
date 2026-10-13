import {toast} from "@/components/ui/toast.tsx";
import {
  FolderView,
  FolderViewProviderContext,
  FolderViewProviderState,
} from "@/lib/contexts/folder-view-context.ts";
import {useNavigation} from "@/lib/contexts/navigation-context.ts";
import {useTranslation} from "@/lib/contexts/translation-context.ts";
import {invoke} from "@tauri-apps/api/core";
import {getCurrentWebview} from "@tauri-apps/api/webview";
import {ReactNode, useCallback, useEffect, useMemo, useState} from "react";

interface FolderViewProviderProps {
  children: ReactNode;
}

const DefaultFolderView: FolderView = {
  path: "",
  files: [],
};

export function FolderViewProvider({
  children,
  ...props
}: FolderViewProviderProps) {
  const {_g} = useTranslation();
  const {setPage} = useNavigation();
  const [folderView, setFolderView] = useState<FolderView>(DefaultFolderView);

  const handleOpenFolder = useCallback(async () => {
    try {
      const newFolderView = await invoke<FolderView>("open_folder");
      setFolderView(newFolderView);
      setPage(newFolderView.path ? "folder" : "home");
    } catch (error) {
      toast.add({
        type: "error",
        title: _g("Error"),
        description: String(error),
      });
    }
  }, [_g, setPage]);

  const handleCloseFolder = useCallback(async () => {
    setFolderView(await invoke("close_folder"));
    setPage("home");
    toast.add({
      type: "info",
      title: _g("Folder closed"),
    });
  }, [_g, setPage]);

  useEffect(() => {
    const unlisten = getCurrentWebview().onDragDropEvent(async (event) => {
      if (event.payload.type !== "drop" || event.payload.paths.length === 0) {
        return;
      }
      try {
        const newFolderView = await invoke<FolderView>("open_folder", {
          path: event.payload.paths[0],
        });
        setFolderView(newFolderView);
        setPage("folder");
      } catch (error) {
        toast.add({
          type: "error",
          title: _g("Error"),
          description: String(error),
        });
      }
    });

    return () => {
      void unlisten.then((stop) => stop());
    };
  }, [_g, setPage]);

  return (
    <FolderViewProviderContext.Provider
      {...props}
      value={useMemo<FolderViewProviderState>(
        () => ({
          folderView: folderView,
          openFolder: handleOpenFolder,
          closeFolder: handleCloseFolder,
        }),
        [folderView, handleOpenFolder, handleCloseFolder],
      )}
    >
      {children}
    </FolderViewProviderContext.Provider>
  );
}
