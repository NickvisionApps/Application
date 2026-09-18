import {Button} from "@/components/ui/button.tsx";
import {Empty, EmptyContent, EmptyDescription, EmptyHeader, EmptyMedia, EmptyTitle} from "@/components/ui/empty.tsx";
import {useDialog} from "@/lib/dialog-provider.tsx";
import {useFolderView} from "@/lib/folder-view-provider.tsx";
import {useNavigation} from "@/lib/navigation-provider.tsx";
import {useTranslation} from "@/lib/translation-provider.tsx";
import {ArrowUpRightIcon, LucideIcon, Moon, MoonStar, Sun} from "lucide-react";
import {useEffect, useState} from "react";

interface Greeting {
  label: string,
  icon: LucideIcon
}

export function HomePage() {
  const {_g, _p} = useTranslation();
  const {openDialog} = useDialog();
  const {setPage} = useNavigation();
  const {openFolder} = useFolderView();
  const [greeting, setGreeting] = useState<Greeting>({
    label: _g("Good Day!"),
    icon: Sun
  });

  useEffect(() => {
    const hours = new Date().getHours();
    let label = "";
    let icon = Sun;
    if (hours < 6) {
      label = _p("Night", "Good Morning!");
      icon = MoonStar;
    } else if (hours < 12) {
      label = _p("Morning", "Good Morning!");
    } else if (hours < 18) {
      label = _g("Good Afternoon!");
    } else {
      label = _g("Good Evening!");
      icon = Moon
    }
    setGreeting({
      label,
      icon
    })
  }, [])

  return (
    <div className="flex h-full w-full items-center justify-center">
      <Empty>
        <EmptyHeader>
          <EmptyMedia>
            <greeting.icon size={32}/>
          </EmptyMedia>
          <EmptyTitle>{greeting.label}</EmptyTitle>
          <EmptyDescription>Open a folder (or drag one in) to get started!</EmptyDescription>
        </EmptyHeader>
        <EmptyContent>
          <Button onClick={async () => {
            if (await openFolder()) {
              setPage("folder");
            }
          }}>{_g("Open Folder")}</Button>
        </EmptyContent>
        <Button variant="link" className=" text-xs text-muted-foreground" size="sm"
                onClick={() => openDialog("changelog")}>
          {_g("What's New?")}
          <ArrowUpRightIcon/>
        </Button>
      </Empty>
    </div>
  )
}