import {toast} from "@/components/ui/toast.tsx";
import {useConfiguration} from "@/lib/contexts/configuration-context.ts";
import {useTranslation} from "@/lib/contexts/translation-context.ts";
import {
  UpdateInformation,
  UpdateProviderContext,
  UpdateProviderState,
} from "@/lib/contexts/update-context.ts";
import {invoke} from "@tauri-apps/api/core";
import {ReactNode, useCallback, useEffect, useMemo, useState} from "react";

interface UpdateProviderProps {
  children: ReactNode;
}

export function UpdateProvider({children, ...props}: UpdateProviderProps) {
  const {configuration} = useConfiguration();
  const {_g, _p} = useTranslation();
  const [checkingForUpdates, setCheckingForUpdates] = useState(false);
  const [updateInformation, setUpdateInformation] =
    useState<UpdateInformation | null>(null);

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
  }, [updateInformation]);

  const installUpdate = useCallback(async () => {
    if (!updateInformation) {
      return;
    }
    try {
      setUpdateInformation(await invoke("install_update"));
    } catch (error) {
      toast.add({
        type: "error",
        title: _g("Error"),
        description: String(error),
      });
    }
  }, [updateInformation]);

  return (
    <UpdateProviderContext.Provider
      {...props}
      value={useMemo<UpdateProviderState>(
        () => ({
          checkingForUpdates,
          updateInformation,
          checkForUpdates,
          installUpdate,
        }),
        [checkingForUpdates, updateInformation, checkForUpdates, installUpdate],
      )}
    >
      {children}
    </UpdateProviderContext.Provider>
  );
}
