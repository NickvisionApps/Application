import {FolderPage} from "@/components/pages/folder-page.tsx";
import {HomePage} from "@/components/pages/home-page.tsx";
import {useNavigation} from "@/lib/navigation-provider.tsx";

export function ViewSwitcher() {
  const {page} = useNavigation();

  return (
    <>
      {page === "home" && <HomePage/>}
      {page === "folder" && <FolderPage/>}
    </>
  )
}