import {invoke} from "@tauri-apps/api/core";
import {createContext, ReactNode, useCallback, useContext, useMemo, useState} from "react";

interface FolderView {
  path: string,
  files: string[]
}

interface FolderViewProviderProps {
  children: ReactNode
}

interface FolderViewProviderState {
  folderView: FolderView,
  openFolder: () => Promise<boolean>,
  closeFolder: () => Promise<void>
}

const DefaultFolderView: FolderView = {
  path: "",
  files: []
};

const FolderViewProviderContext = createContext<FolderViewProviderState>({
  folderView: DefaultFolderView,
  openFolder: async () => {
    return false;
  },
  closeFolder: async () => {
  }
});

export function FolderViewProvider({children, ...props}: FolderViewProviderProps) {
  const [folderView, setFolderView] = useState<FolderView>(DefaultFolderView);

  const handleOpenFolder = useCallback(async () => {
    try {
      setFolderView(await invoke("open_folder"));
      return true;
    } catch {
      return false;
    }
  }, []);

  const handleCloseFolder = useCallback(async () => {
    await invoke("close_folder");
    setFolderView(DefaultFolderView);
  }, []);

  return (
    <FolderViewProviderContext.Provider {...props} value={useMemo<FolderViewProviderState>(() => ({
      folderView: folderView,
      openFolder: handleOpenFolder,
      closeFolder: handleCloseFolder
    }), [folderView, handleOpenFolder, handleCloseFolder])}>
      {children}
    </FolderViewProviderContext.Provider>
  )
}

export const useFolderView = () => {
  const context = useContext(FolderViewProviderContext);
  if (!context) {
    throw new Error("useFolderView must be used with a FolderViewProvider");
  }
  return context;
}