import {toast} from "@/components/ui/toast.tsx";
import {useConfiguration} from "@/lib/contexts/configuration-context.ts";
import {useTranslation} from "@/lib/contexts/translation-context.ts";
import {
  UpdateDownloadProgress,
  UpdateInformation,
  UpdateProviderContext,
  UpdateProviderState,
} from "@/lib/contexts/update-context.ts";
import {invoke} from "@tauri-apps/api/core";
import {listen} from "@tauri-apps/api/event";
import {ReactNode, useCallback, useEffect, useMemo, useState} from "react";

interface UpdateProviderProps {
  children: ReactNode;
}

export function UpdateProvider({children, ...props}: UpdateProviderProps) {
  const {configuration} = useConfiguration();
  const {_g, _p} = useTranslation();
  const [checkingForUpdates, setCheckingForUpdates] = useState(false);
  const [installingUpdate, setInstallingUpdate] = useState(false);
  const [updateDownloadProgress, setUpdateDownloadProgress] =
    useState<UpdateDownloadProgress | null>(null);
  const [updateInformation, setUpdateInformation] =
    useState<UpdateInformation | null>(null);

  useEffect(() => {
    const unlisten = listen<UpdateDownloadProgress>(
      "update-download-progress",
      (event) => setUpdateDownloadProgress(event.payload),
    );

    return () => {
      void unlisten.then((stop) => stop());
    };
  }, []);

  useEffect(() => {
    async function startup() {
      setCheckingForUpdates(true);
      try {
        setUpdateInformation(await invoke("get_new_update"));
      } catch (error) {
        toast.add({
          type: "error",
          title: _g("Error"),
          description: String(error),
        });
      } finally {
        setCheckingForUpdates(false);
      }
    }

    if (!configuration.automaticallyCheckForUpdates) {
      return;
    }
    void startup();
  }, [configuration.automaticallyCheckForUpdates]);

  const checkForUpdates = useCallback(async () => {
    if (installingUpdate) {
      return;
    }
    setCheckingForUpdates(true);
    setUpdateInformation(null);
    try {
      let update = await invoke<UpdateInformation | null>("get_new_update");
      setUpdateInformation(update);
      if (!update) {
        toast.add({
          type: "info",
          title: _g("No Update Available"),
          description: _p(
            "AppName",
            "You are already using the latest version of Application",
          ),
        });
      }
    } catch (error) {
      toast.add({
        type: "error",
        title: _g("Error"),
        description: String(error),
      });
    } finally {
      setCheckingForUpdates(false);
    }
  }, [installingUpdate]);

  const installUpdate = useCallback(async () => {
    if (!updateInformation || installingUpdate) {
      return;
    }
    setInstallingUpdate(true);
    setUpdateDownloadProgress(null);
    try {
      await invoke("install_update");
    } catch (error) {
      toast.add({
        type: "error",
        title: _g("Error"),
        description: String(error),
      });
    } finally {
      setInstallingUpdate(false);
    }
  }, [updateInformation, installingUpdate]);

  return (
    <UpdateProviderContext.Provider
      {...props}
      value={useMemo<UpdateProviderState>(
        () => ({
          checkingForUpdates,
          installingUpdate,
          updateDownloadProgress,
          updateInformation,
          checkForUpdates,
          installUpdate,
        }),
        [
          checkingForUpdates,
          installingUpdate,
          updateDownloadProgress,
          updateInformation,
          checkForUpdates,
          installUpdate,
        ],
      )}
    >
      {children}
    </UpdateProviderContext.Provider>
  );
}
