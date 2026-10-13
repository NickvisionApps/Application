import {HStack, VStack} from "@/components/layout/stack.tsx";
import {Button} from "@/components/ui/button.tsx";
import {Kbd, KbdGroup} from "@/components/ui/kbd.tsx";
import {
  Tooltip,
  TooltipContent,
  TooltipTrigger,
} from "@/components/ui/tooltip.tsx";
import {useDialog} from "@/lib/contexts/dialog-context.ts";
import {useFolderView} from "@/lib/contexts/folder-view-context.ts";
import {useTranslation} from "@/lib/contexts/translation-context.ts";
import {platform} from "@tauri-apps/plugin-os";
import {
  FolderOpen,
  LucideIcon,
  Moon,
  MoonStar,
  Settings,
  Sparkles,
  Sun,
} from "lucide-react";
import {useEffect, useState} from "react";

interface Greeting {
  label: string;
  icon: LucideIcon;
}

export function HomePage() {
  const {_g, _p} = useTranslation();
  const {openDialog} = useDialog();
  const {openFolder} = useFolderView();
  const [greeting, setGreeting] = useState<Greeting>({
    label: _g("Good Day!"),
    icon: Sun,
  });

  useEffect(() => {
    const hours = new Date().getHours();
    let label: string;
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
      icon = Moon;
    }
    setGreeting({
      label,
      icon,
    });
  }, []);

  return (
    <div className="relative h-full w-full overflow-hidden">
      <svg
        aria-hidden="true"
        viewBox="0 0 800 600"
        preserveAspectRatio="xMidYMid slice"
        className="pointer-events-none absolute inset-0 size-full text-foreground"
      >
        <defs>
          <radialGradient id="home-glow">
            <stop offset="0%" stopColor="currentColor" stopOpacity="1" />
            <stop offset="100%" stopColor="currentColor" stopOpacity="0" />
          </radialGradient>
        </defs>
        <g opacity="0.1">
          <circle cx="680" cy="90" r="280" fill="url(#home-glow)" />
          <circle cx="90" cy="540" r="240" fill="url(#home-glow)" />
        </g>
        <g opacity="0.14" fill="none" stroke="currentColor" strokeWidth="1">
          <circle cx="680" cy="90" r="80" />
          <circle cx="680" cy="90" r="140" />
          <circle cx="680" cy="90" r="200" />
          <circle cx="90" cy="540" r="60" />
          <circle cx="90" cy="540" r="120" />
          <circle cx="90" cy="540" r="180" />
          <rect
            x="560"
            y="400"
            width="120"
            height="120"
            rx="30"
            transform="rotate(18 620 460)"
          />
          <rect
            x="110"
            y="70"
            width="70"
            height="70"
            rx="20"
            transform="rotate(-14 145 105)"
          />
          <path d="M0 300 C200 190 420 430 800 250" />
          <path d="M0 330 C200 220 420 460 800 280" />
        </g>
        <g opacity="0.2" fill="currentColor">
          <circle cx="470" cy="120" r="4" />
          <circle cx="250" cy="470" r="3" />
          <circle cx="720" cy="360" r="3" />
        </g>
      </svg>
      <VStack
        align="center"
        justify="center"
        gap={6}
        className="relative z-10 h-full w-full overflow-y-auto p-8"
      >
        <VStack align="center" gap={3} className="text-center">
          <div className="flex size-12 items-center justify-center rounded-2xl bg-foreground/5 ring-1 ring-foreground/10">
            <greeting.icon size={24} />
          </div>
          <h1 className="text-xl font-semibold tracking-tight">
            {greeting.label}
          </h1>
          <p className="max-w-md text-sm text-muted-foreground">
            {_g("Open a folder (or drag one in) to get started!")}
          </p>
        </VStack>
        <Tooltip>
          <TooltipTrigger
            render={
              <Button
                onClick={() => {
                  void openFolder();
                }}
              >
                <FolderOpen />
                {_g("Open Folder")}
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
        <HStack align="center" justify="center" gap={1}>
          <Tooltip>
            <TooltipTrigger
              render={
                <Button
                  variant="ghost"
                  size="sm"
                  className="text-xs text-muted-foreground"
                  onClick={() => {
                    openDialog("changelog");
                  }}
                >
                  <Sparkles />
                  {_g("What's New?")}
                </Button>
              }
            />
            <TooltipContent>
              {_g("What's New?")}
              <Kbd>F1</Kbd>
            </TooltipContent>
          </Tooltip>
          <Tooltip>
            <TooltipTrigger
              render={
                <Button
                  variant="ghost"
                  size="sm"
                  className="text-xs text-muted-foreground"
                  onClick={() => {
                    openDialog("settings");
                  }}
                >
                  <Settings />
                  {_g("Settings")}
                </Button>
              }
            />
            <TooltipContent>
              {_g("Settings")}
              <KbdGroup>
                {platform() === "macos" && <Kbd>⌘</Kbd>}
                {platform() !== "macos" && <Kbd>Ctrl</Kbd>}
                <Kbd>,</Kbd>
              </KbdGroup>
            </TooltipContent>
          </Tooltip>
        </HStack>
      </VStack>
    </div>
  );
}
