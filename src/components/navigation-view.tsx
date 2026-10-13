import {Avatar, AvatarFallback} from "@/components/ui/avatar.tsx";
import {Badge} from "@/components/ui/badge.tsx";
import {
  DropdownMenu,
  DropdownMenuContent,
  DropdownMenuGroup,
  DropdownMenuItem,
  DropdownMenuPortal,
  DropdownMenuRadioGroup,
  DropdownMenuRadioItem,
  DropdownMenuSeparator,
  DropdownMenuShortcut,
  DropdownMenuSub,
  DropdownMenuSubContent,
  DropdownMenuSubTrigger,
  DropdownMenuTrigger,
} from "@/components/ui/dropdown-menu.tsx";
import {Item, ItemContent, ItemMedia} from "@/components/ui/item.tsx";
import {Progress} from "@/components/ui/progress.tsx";
import {
  Sidebar,
  SidebarContent,
  SidebarFooter,
  SidebarGroup,
  SidebarGroupContent,
  SidebarGroupLabel,
  SidebarMenu,
  SidebarMenuButton,
  SidebarMenuItem,
  useSidebar,
} from "@/components/ui/sidebar.tsx";
import {Spinner} from "@/components/ui/spinner.tsx";
import {
  Tooltip,
  TooltipContent,
  TooltipTrigger,
} from "@/components/ui/tooltip.tsx";
import {Theme, useConfiguration} from "@/lib/contexts/configuration-context.ts";
import {useDialog} from "@/lib/contexts/dialog-context.ts";
import {useFolderView} from "@/lib/contexts/folder-view-context.ts";
import {Page, useNavigation} from "@/lib/contexts/navigation-context.ts";
import {useTranslation} from "@/lib/contexts/translation-context.ts";
import {useUpdate} from "@/lib/contexts/update-context.ts";
import {invoke} from "@tauri-apps/api/core";
import {platform} from "@tauri-apps/plugin-os";
import {
  BadgeInfo,
  BadgePlus,
  Bug,
  ChevronsUpDown,
  Folder,
  Home,
  LifeBuoy,
  LucideIcon,
  MessageSquareShare,
  RefreshCw,
  Settings,
  Sparkles,
  SquareLibrary,
  SunMoon,
  UserGroup,
} from "lucide-react";
import {useEffect, useState} from "react";

interface NavigationViewItem {
  title: string;
  page: Page;
  icon: LucideIcon;
}

interface NavigationViewSection {
  title: string;
  items: (NavigationViewItem | null)[];
}

interface User {
  username: string;
  fullName: string;
}

export function NavigationView() {
  const {configuration, setConfiguration} = useConfiguration();
  const {_f, _g, _p} = useTranslation();
  const {
    checkingForUpdates,
    checkForUpdates,
    installingUpdate,
    updateDownloadProgress,
    updateInformation,
  } = useUpdate();
  const {openDialog} = useDialog();
  const {page, setPage} = useNavigation();
  const {isMobile} = useSidebar();
  const {folderView} = useFolderView();
  const [user, setUser] = useState<User>({
    username: "",
    fullName: "",
  });

  const items: NavigationViewSection[] = [
    {
      title: "",
      items: [
        {
          title: _g("Home"),
          page: "home",
          icon: Home,
        },
        folderView.path
          ? {
              title: _g("Folder"),
              page: "folder",
              icon: Folder,
            }
          : null,
      ],
    },
  ];

  useEffect(() => {
    async function startup() {
      setUser(await invoke("get_user"));
    }

    void startup();
  }, []);

  return (
    <Sidebar>
      <SidebarContent className={isMobile ? "mt-8" : "pt-(--titlebar-height)"}>
        {items.map((section) => (
          <SidebarGroup key={section.title}>
            {section.title && (
              <SidebarGroupLabel>{section.title}</SidebarGroupLabel>
            )}
            <SidebarGroupContent>
              <SidebarMenu>
                {section.items
                  .filter((item) => item !== null)
                  .map((item) => (
                    <SidebarMenuItem key={item.page}>
                      <SidebarMenuButton
                        tooltip={item.title}
                        isActive={page === item.page}
                        onClick={() => {
                          setPage(item.page);
                        }}
                      >
                        <item.icon />
                        <span>{item.title}</span>
                      </SidebarMenuButton>
                    </SidebarMenuItem>
                  ))}
              </SidebarMenu>
            </SidebarGroupContent>
          </SidebarGroup>
        ))}
      </SidebarContent>
      <SidebarFooter>
        <SidebarMenu
          className={updateInformation || installingUpdate ? "gap-2" : ""}
        >
          {checkingForUpdates && (
            <Item>
              <ItemMedia>
                <Spinner className="size-3" />
              </ItemMedia>
              <ItemContent>
                <span className="truncated text-xs text-muted-foreground">
                  {_g("Checking for updates...")}
                </span>
              </ItemContent>
            </Item>
          )}
          {installingUpdate && (
            <Tooltip>
              <TooltipTrigger render={<Item />}>
                {(!updateDownloadProgress ||
                  updateDownloadProgress.total === 0) && (
                  <ItemMedia>
                    <Spinner className="size-3" />
                  </ItemMedia>
                )}
                <ItemContent>
                  <div className="flex items-center justify-between gap-2 text-xs text-muted-foreground">
                    <span className="truncated">
                      {_g("Downloading update...")}
                    </span>
                    {updateDownloadProgress &&
                      updateDownloadProgress.total > 0 && (
                        <span className="tabular-nums">
                          {Math.round(
                            (updateDownloadProgress.downloaded /
                              updateDownloadProgress.total) *
                              100,
                          )}
                          %
                        </span>
                      )}
                  </div>
                  {updateDownloadProgress &&
                    updateDownloadProgress.total > 0 && (
                      <Progress
                        value={
                          (updateDownloadProgress.downloaded /
                            updateDownloadProgress.total) *
                          100
                        }
                      />
                    )}
                </ItemContent>
              </TooltipTrigger>
              <TooltipContent side="right">
                {updateDownloadProgress && updateDownloadProgress.total > 0
                  ? _f("Downloading update... {0}% ({1} MB of {2} MB)", [
                      Math.round(
                        (updateDownloadProgress.downloaded /
                          updateDownloadProgress.total) *
                          100,
                      ).toString(),
                      (updateDownloadProgress.downloaded / 1048576).toFixed(1),
                      (updateDownloadProgress.total / 1048576).toFixed(1),
                    ])
                  : _g("Downloading update...")}
              </TooltipContent>
            </Tooltip>
          )}
          {updateInformation && !installingUpdate && (
            <SidebarMenuItem>
              <SidebarMenuButton
                className="bg-primary text-primary-foreground hover:bg-primary/90 hover:text-primary-foreground"
                onClick={() => openDialog("update")}
              >
                <BadgePlus />
                <span className="truncated">{_g("Update Available")}</span>
                <Badge variant="secondary">{updateInformation.version}</Badge>
              </SidebarMenuButton>
            </SidebarMenuItem>
          )}
          <SidebarMenuItem>
            <DropdownMenu>
              <DropdownMenuTrigger render={<SidebarMenuButton size="lg" />}>
                {user.fullName && (
                  <>
                    <Avatar>
                      <AvatarFallback>
                        {user.fullName
                          .split(" ")
                          .map((word) => word.charAt(0))
                          .join("")}
                      </AvatarFallback>
                    </Avatar>
                    <div className="grid flex-1 text-left text-sm leading-tight">
                      <span className="truncated font-medium">
                        {user.fullName}
                      </span>
                      {user.username !== user.fullName && (
                        <span className="truncated text-xs text-muted-foreground">
                          {user.username}
                        </span>
                      )}
                    </div>
                    <ChevronsUpDown />
                  </>
                )}
                {!user.fullName && <Spinner />}
              </DropdownMenuTrigger>
              <DropdownMenuContent className="min-w-56">
                <DropdownMenuGroup>
                  <DropdownMenuItem
                    onClick={() => {
                      void checkForUpdates();
                    }}
                  >
                    <RefreshCw />
                    <span>{_g("Check for Updates")}</span>
                    {platform() === "macos" && (
                      <DropdownMenuShortcut>⌘R</DropdownMenuShortcut>
                    )}
                    {platform() !== "macos" && (
                      <DropdownMenuShortcut>Ctrl+R</DropdownMenuShortcut>
                    )}
                  </DropdownMenuItem>
                  <DropdownMenuSub>
                    <DropdownMenuSubTrigger>
                      <LifeBuoy />
                      <span>{_g("Help")}</span>
                    </DropdownMenuSubTrigger>
                    <DropdownMenuPortal>
                      <DropdownMenuSubContent>
                        <DropdownMenuGroup>
                          <DropdownMenuItem
                            onClick={() => {
                              openDialog("changelog");
                            }}
                          >
                            <Sparkles />
                            <span>{_g("What's New?")}</span>
                            <DropdownMenuShortcut>F1</DropdownMenuShortcut>
                          </DropdownMenuItem>
                        </DropdownMenuGroup>
                        <DropdownMenuSeparator />
                        <DropdownMenuGroup>
                          <DropdownMenuItem
                            onClick={() => {
                              void invoke("open_github_repository");
                            }}
                          >
                            <SquareLibrary />
                            <span>{_g("GitHub Repository")}</span>
                          </DropdownMenuItem>
                          <DropdownMenuItem
                            onClick={() => {
                              void invoke("open_report_a_bug");
                            }}
                          >
                            <Bug />
                            <span>{_g("Report a Bug")}</span>
                          </DropdownMenuItem>
                          <DropdownMenuItem
                            onClick={() => {
                              void invoke("open_discussions");
                            }}
                          >
                            <MessageSquareShare />
                            <span>{_g("Discussions")}</span>
                          </DropdownMenuItem>
                        </DropdownMenuGroup>
                        <DropdownMenuSeparator />
                        <DropdownMenuGroup>
                          <DropdownMenuItem
                            onClick={() => {
                              openDialog("credits");
                            }}
                          >
                            <UserGroup />
                            <span>{_g("Credits")}</span>
                          </DropdownMenuItem>
                          <DropdownMenuItem
                            onClick={() => {
                              openDialog("debugging");
                            }}
                          >
                            <BadgeInfo />
                            <span>{_g("Debugging Information")}</span>
                            {platform() === "macos" && (
                              <DropdownMenuShortcut>⌘⇧D</DropdownMenuShortcut>
                            )}
                            {platform() !== "macos" && (
                              <DropdownMenuShortcut>
                                Ctrl+Shift+D
                              </DropdownMenuShortcut>
                            )}
                          </DropdownMenuItem>
                        </DropdownMenuGroup>
                      </DropdownMenuSubContent>
                    </DropdownMenuPortal>
                  </DropdownMenuSub>
                </DropdownMenuGroup>
                <DropdownMenuSeparator />
                <DropdownMenuGroup>
                  <DropdownMenuSub>
                    <DropdownMenuSubTrigger>
                      <SunMoon />
                      <span>{_g("Theme")}</span>
                    </DropdownMenuSubTrigger>
                    <DropdownMenuPortal>
                      <DropdownMenuSubContent>
                        <DropdownMenuRadioGroup
                          value={configuration.theme}
                          onValueChange={(value) => {
                            setConfiguration({
                              ...configuration,
                              theme: value as Theme,
                            });
                          }}
                        >
                          <DropdownMenuRadioItem value={0}>
                            {_p("Theme", "Light")}
                          </DropdownMenuRadioItem>
                          <DropdownMenuRadioItem value={1}>
                            {_p("Theme", "Dark")}
                          </DropdownMenuRadioItem>
                          <DropdownMenuRadioItem value={2}>
                            {_p("Theme", "System")}
                          </DropdownMenuRadioItem>
                        </DropdownMenuRadioGroup>
                      </DropdownMenuSubContent>
                    </DropdownMenuPortal>
                  </DropdownMenuSub>
                  <DropdownMenuItem
                    onClick={() => {
                      openDialog("settings");
                    }}
                  >
                    <Settings />
                    <span>{_g("Settings")}</span>
                    {platform() === "macos" && (
                      <DropdownMenuShortcut>⌘,</DropdownMenuShortcut>
                    )}
                    {platform() !== "macos" && (
                      <DropdownMenuShortcut>Ctrl+,</DropdownMenuShortcut>
                    )}
                  </DropdownMenuItem>
                </DropdownMenuGroup>
              </DropdownMenuContent>
            </DropdownMenu>
          </SidebarMenuItem>
        </SidebarMenu>
      </SidebarFooter>
    </Sidebar>
  );
}
