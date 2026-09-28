import {toast} from "@/components/ui/toast.tsx";
import {
  FolderView,
  FolderViewProviderContext,
  FolderViewProviderState,
} from "@/lib/contexts/folder-view-context.ts";
import {useNavigation} from "@/lib/contexts/navigation-context.ts";
import {useTranslation} from "@/lib/contexts/translation-context.ts";
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
  const {_g} = useTranslation();
  const {setPage} = useNavigation();
  const [folderView, setFolderView] = useState<FolderView>(DefaultFolderView);

  const handleOpenFolder = useCallback(async () => {
    try {
      setFolderView(await invoke("open_folder"));
      setPage("folder");
    } catch {}
  }, []);

  const handleCloseFolder = useCallback(async () => {
    setFolderView(await invoke("close_folder"));
    setPage("home");
    toast.add({
      type: "info",
      title: _g("Folder closed"),
    });
  }, []);

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
