use crate::AppInfo;
use gettext::Catalog;
use std::fs::File;

pub struct Translator {
    language: String,
    catalog: Catalog,
}

impl Translator {
    pub fn new<T: Into<String>>(language: T) -> Self {
        let mut language = language.into();
        if language.is_empty() || language == "C" {
            language = std::env::var("LANG")
                .unwrap_or_else(|_| "C".to_string())
                .split('.')
                .next()
                .unwrap_or("C")
                .to_string();
        }
        if language == "C" {
            language = "en_US".to_string();
        }
        Translator {
            language: language.clone(),
            catalog: if language == "en_US" {
                Catalog::empty()
            } else {
                let file = File::open(
                    std::env::current_exe()
                        .unwrap()
                        .parent()
                        .unwrap()
                        .join(&language)
                        .join("LC_MESSAGES")
                        .join(format!(
                            "{}.mo",
                            AppInfo::default().short_name().to_lowercase()
                        )),
                )
                .unwrap();
                Catalog::parse(file).unwrap()
            },
        }
    }

    pub fn language(&self) -> &str {
        &self.language
    }

    pub fn _g(&self, msgid: &str) -> String {
        self.catalog.gettext(msgid).to_string()
    }

    pub fn _f(&self, msgid: &str, args: &[&str]) -> String {
        let translated = self.catalog.gettext(msgid);
        let mut result = translated.to_string();
        for (i, arg) in args.iter().enumerate() {
            let placeholder = format!("{{{}}}", i);
            result = result.replace(&placeholder, arg);
        }
        result
    }

    pub fn _n(&self, msgid: &str, msgid_plural: &str, n: u64) -> String {
        self.catalog.ngettext(msgid, msgid_plural, n).to_string()
    }

    pub fn _nf(&self, msgid: &str, msgid_plural: &str, n: u64, args: &[&str]) -> String {
        let translated = self.catalog.ngettext(msgid, msgid_plural, n);
        let mut result = translated.to_string();
        for (i, arg) in args.iter().enumerate() {
            let placeholder = format!("{{{}}}", i);
            result = result.replace(&placeholder, arg);
        }
        result
    }

    pub fn _p(&self, msgctxt: &str, msgid: &str) -> String {
        self.catalog.pgettext(msgctxt, msgid).to_string()
    }

    pub fn _pf(&self, msgctxt: &str, msgid: &str, args: &[&str]) -> String {
        let translated = self.catalog.pgettext(msgctxt, msgid);
        let mut result = translated.to_string();
        for (i, arg) in args.iter().enumerate() {
            let placeholder = format!("{{{}}}", i);
            result = result.replace(&placeholder, arg);
        }
        result
    }

    pub fn _np(&self, msgctxt: &str, msgid: &str, msgid_plural: &str, n: u64) -> String {
        self.catalog
            .npgettext(msgctxt, msgid, msgid_plural, n)
            .to_string()
    }

    pub fn _npf(
        &self,
        msgctxt: &str,
        msgid: &str,
        msgid_plural: &str,
        n: u64,
        args: &[&str],
    ) -> String {
        let translated = self.catalog.npgettext(msgctxt, msgid, msgid_plural, n);
        let mut result = translated.to_string();
        for (i, arg) in args.iter().enumerate() {
            let placeholder = format!("{{{}}}", i);
            result = result.replace(&placeholder, arg);
        }
        result
    }
}
