import {HStack, VStack} from "@/components/layout/stack.tsx";
import {Avatar, AvatarFallback} from "@/components/ui/avatar.tsx";
import {
  DropdownMenu,
  DropdownMenuContent,
  DropdownMenuGroup,
  DropdownMenuItem,
  DropdownMenuPortal,
  DropdownMenuRadioGroup,
  DropdownMenuRadioItem,
  DropdownMenuSeparator,
  DropdownMenuSub,
  DropdownMenuSubContent,
  DropdownMenuSubTrigger,
  DropdownMenuTrigger,
} from "@/components/ui/dropdown-menu.tsx";
import {
  Sidebar,
  SidebarContent,
  SidebarFooter,
  SidebarGroup,
  SidebarGroupContent,
  SidebarGroupLabel,
  SidebarHeader,
  SidebarMenu,
  SidebarMenuButton,
  SidebarMenuItem,
  useSidebar,
} from "@/components/ui/sidebar.tsx";
import {Spinner} from "@/components/ui/spinner.tsx";
import {Theme, useConfiguration} from "@/lib/configuration-context.ts";
import {useDialog} from "@/lib/dialog-context.ts";
import {useFolderView} from "@/lib/folder-view-context.ts";
import {Page, useNavigation} from "@/lib/navigation-context.ts";
import {useProductInfo} from "@/lib/product-info-context.ts";
import {useTranslation} from "@/lib/translation-context.ts";
import {invoke} from "@tauri-apps/api/core";
import {
  BadgeInfo,
  Bug,
  ChevronsUpDown,
  Folder,
  Home,
  LifeBuoy,
  LucideIcon,
  MessageSquareShare,
  Settings,
  Sparkles,
  SquareLibrary,
  SunMoon,
  UserGroup,
} from "lucide-react";
import {useEffect, useState} from "react";

import logoDevSrc from "../../resources/org.nickvision.application-devel.svg";
import logoSrc from "../../resources/org.nickvision.application.svg";

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
  const {productInfo} = useProductInfo();
  const {configuration, setConfiguration} = useConfiguration();
  const {_f, _g, _p} = useTranslation();
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
      <SidebarHeader
        className={
          isMobile ? "mt-8" : "pt-(--tauri-plugin-decoration-titlebar-height)"
        }
      >
        <SidebarMenu>
          <SidebarMenuItem>
            <SidebarMenuButton size="lg">
              <HStack gap={3} align="center">
                <VStack align="center" justify="center" className="size-8">
                  <img
                    src={
                      productInfo.version.includes("-") ? logoDevSrc : logoSrc
                    }
                    alt=""
                    className="size-8 rounded-lg"
                  />
                </VStack>
                <VStack gap={0.5}>
                  <span className="font-medium">
                    {_p("AppName", "Application")}
                  </span>
                  {productInfo.version.includes("-") && (
                    <span className="text-xs text-muted-foreground">
                      {_f("Preview ({0})", [productInfo.version])}
                    </span>
                  )}
                </VStack>
              </HStack>
            </SidebarMenuButton>
          </SidebarMenuItem>
        </SidebarMenu>
      </SidebarHeader>
      <SidebarContent>
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
        <SidebarMenu>
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
                      openDialog("changelog");
                    }}
                  >
                    <Sparkles />
                    <span>{_g("What's New?")}</span>
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
