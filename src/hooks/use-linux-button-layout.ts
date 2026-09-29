import {invoke} from "@tauri-apps/api/core";
import {platform} from "@tauri-apps/plugin-os";
import {useEffect, useState} from "react";

export type WindowButtonId = "minimize" | "maximize" | "close";

interface LinuxButtonLayout {
  side: "left" | "right";
  order: WindowButtonId[];
}

const DEFAULT_LAYOUT: LinuxButtonLayout = {
  side: "right",
  order: ["minimize", "maximize", "close"],
};

export function useLinuxButtonLayout() {
  const [layout, setLayout] = useState(DEFAULT_LAYOUT);

  useEffect(() => {
    if (platform() !== "linux") {
      return;
    }

    async function startup() {
      const raw = await invoke<string | null>("get_linux_button_layout");
      if (!raw) {
        return;
      }
      const [leftRaw, rightRaw] = raw.includes(":")
        ? raw.split(":")
        : ["", raw];
      const parse = (tokens: string) =>
        tokens
          .split(",")
          .map((token) => token.trim())
          .filter(
            (token): token is WindowButtonId =>
              token === "minimize" ||
              token === "maximize" ||
              token === "close",
          );
      const left = parse(leftRaw);
      const right = parse(rightRaw);
      if (left.length > 0) {
        setLayout({side: "left", order: left});
      } else if (right.length > 0) {
        setLayout({side: "right", order: right});
      }
    }

    void startup();
  }, []);

  return layout;
}
