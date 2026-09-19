import {invoke, InvokeArgs} from "@tauri-apps/api/core";
import {
  createContext,
  ReactNode,
  useCallback,
  useContext,
  useEffect,
  useMemo,
  useState,
} from "react";

import {useConfiguration} from "@/lib/configuration-provider.tsx";

interface TranslationProviderProps {
  children: ReactNode;
}

interface TranslationProviderState {
  _g: (msgid: string) => string;
  _f: (msgid: string, args: string[]) => string;
  _n: (msgid: string, msgidPlural: string, n: number) => string;
  _nf: (
    msgid: string,
    msgidPlural: string,
    n: number,
    args: string[],
  ) => string;
  _p: (msgctxt: string, msgid: string) => string;
  _pf: (msgctxt: string, msgid: string, args: string[]) => string;
  _np: (
    msgctxt: string,
    msgid: string,
    msgidPlural: string,
    n: number,
  ) => string;
  _npf: (
    msgctxt: string,
    msgid: string,
    msgidPlural: string,
    n: number,
    args: string[],
  ) => string;
}

const TranslationProviderContext = createContext<
  TranslationProviderState | undefined
>(undefined);

export function TranslationProvider({
  children,
  ...props
}: TranslationProviderProps) {
  const {configuration} = useConfiguration();
  const [cache, setCache] = useState<Partial<Record<string, string>>>({});

  useEffect(() => {
    setCache({});
  }, [configuration.translationLanguage]);

  const fetchTranslation = useCallback(
    (key: string, cmd: string, args: InvokeArgs) => {
      if (key in cache) {
        return;
      }
      invoke(cmd, args)
        .then((value: unknown) => {
          const translation = value as string;
          setCache((prev) => ({
            ...prev,
            [key]: translation,
          }));
        })
        .catch(() => {
          setCache((prev) => ({
            ...prev,
            [key]: key.split("::").pop() || "",
          }));
        });
    },
    [cache],
  );

  return (
    <TranslationProviderContext.Provider
      {...props}
      value={useMemo<TranslationProviderState>(
        () => ({
          _g: (msgid: string): string => {
            const key = `g::${msgid}`;
            if (cache[key] !== undefined) {
              return cache[key];
            }
            fetchTranslation(key, "translate_g", {msgid});
            return msgid;
          },
          _f: (msgid: string, args: string[]): string => {
            const key = `f::${args.join(",")}::${msgid}`;
            if (cache[key] !== undefined) {
              return cache[key];
            }
            fetchTranslation(key, "translate_f", {msgid, args});
            return msgid;
          },
          _n: (msgid: string, msgidPlural: string, n: number): string => {
            const fallback = n === 1 ? msgid : msgidPlural;
            const key = `n::${msgid}::${msgidPlural}::${n}::${fallback}`;
            if (cache[key] !== undefined) {
              return cache[key];
            }
            fetchTranslation(key, "translate_n", {msgid, msgidPlural, n});
            return fallback;
          },
          _nf: (
            msgid: string,
            msgidPlural: string,
            n: number,
            args: string[],
          ): string => {
            const fallback = n === 1 ? msgid : msgidPlural;
            const key = `nf::${msgid}::${msgidPlural}::${n}::${args.join(",")}::${fallback}`;
            if (cache[key] !== undefined) {
              return cache[key];
            }
            fetchTranslation(key, "translate_nf", {
              msgid,
              msgidPlural,
              n,
              args,
            });
            return fallback;
          },
          _p: (msgctxt: string, msgid: string): string => {
            const key = `p::${msgctxt}::${msgid}`;
            if (cache[key] !== undefined) {
              return cache[key];
            }
            fetchTranslation(key, "translate_p", {msgctxt, msgid});
            return msgid;
          },
          _pf: (msgctxt: string, msgid: string, args: string[]): string => {
            const key = `pf::${msgctxt}::${args.join(",")}::${msgid}`;
            if (cache[key] !== undefined) {
              return cache[key];
            }
            fetchTranslation(key, "translate_pf", {msgctxt, msgid, args});
            return msgid;
          },
          _np: (
            msgctxt: string,
            msgid: string,
            msgidPlural: string,
            n: number,
          ): string => {
            const fallback = n === 1 ? msgid : msgidPlural;
            const key = `np::${msgctxt}::${msgid}::${msgidPlural}::${n}::${fallback}`;
            if (cache[key] !== undefined) {
              return cache[key];
            }
            fetchTranslation(key, "translate_np", {
              msgctxt,
              msgid,
              msgidPlural,
              n,
            });
            return fallback;
          },
          _npf: (
            msgctxt: string,
            msgid: string,
            msgidPlural: string,
            n: number,
            args: string[],
          ): string => {
            const fallback = n === 1 ? msgid : msgidPlural;
            const key = `npf::${msgctxt}::${msgid}::${msgidPlural}::${n}::${args.join(",")}::${fallback}`;
            if (cache[key] !== undefined) {
              return cache[key];
            }
            fetchTranslation(key, "translate_npf", {
              msgctxt,
              msgid,
              msgidPlural,
              n,
              args,
            });
            return fallback;
          },
        }),
        [cache, fetchTranslation],
      )}
    >
      {children}
    </TranslationProviderContext.Provider>
  );
}

export const useTranslation = () => {
  const context = useContext(TranslationProviderContext);
  if (!context) {
    throw new Error("useTranslation must be used with a TranslationProvider");
  }
  return context;
};
