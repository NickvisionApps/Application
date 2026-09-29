import {getCurrentWindow} from "@tauri-apps/api/window";
import {useEffect, useState} from "react";

export function useWindowMaximized() {
  const [maximized, setMaximized] = useState(false);

  useEffect(() => {
    const window = getCurrentWindow();
    let resizeFn: () => void;

    async function startup() {
      setMaximized(await window.isMaximized());
      resizeFn = await window.onResized(async (_) => {
        setMaximized(await window.isMaximized());
      });
    }

    void startup();

    return () => {
      if (resizeFn) {
        resizeFn();
      }
    };
  }, []);

  return maximized;
}
