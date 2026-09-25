import {
  FolderView,
  FolderViewProviderContext,
  FolderViewProviderState,
} from "@/lib/contexts/folder-view-context.ts";
import {invoke} from "@tauri-apps/api/core";
import {ReactNode, useCallback, useMemo, useState} from "react";

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
  const [folderView, setFolderView] = useState<FolderView>(DefaultFolderView);

  const handleOpenFolder = useCallback(async () => {
    try {
      setFolderView(await invoke("open_folder"));
      return true;
    } catch {
      return false;
    }
  }, []);

  const handleCloseFolder = useCallback(
    async () => setFolderView(await invoke("close_folder")),
    [],
  );

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
