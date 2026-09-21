import {createContext, useContext} from "react";

export interface FolderView {
  path: string;
  files: string[];
}

export interface FolderViewProviderState {
  folderView: FolderView;
  openFolder: () => Promise<boolean>;
  closeFolder: () => Promise<void>;
}

export const FolderViewProviderContext = createContext<
  FolderViewProviderState | undefined
>(undefined);

export const useFolderView = () => {
  const context = useContext(FolderViewProviderContext);
  if (!context) {
    throw new Error("useFolderView must be used with a FolderViewProvider");
  }
  return context;
};
