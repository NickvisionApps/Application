import {createContext, useContext} from "react";

export interface TranslationProviderState {
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

export const TranslationProviderContext = createContext<
  TranslationProviderState | undefined
>(undefined);

export const useTranslation = () => {
  const context = useContext(TranslationProviderContext);
  if (!context) {
    throw new Error("useTranslation must be used with a TranslationProvider");
  }
  return context;
};
