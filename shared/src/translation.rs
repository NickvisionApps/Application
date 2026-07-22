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
            language = std::env::var("LC_ALL")
                .or_else(|_| std::env::var("LC_MESSAGES"))
                .or_else(|_| std::env::var("LANG"))
                .ok()
                .or_else(sys_locale::get_locale)
                .unwrap_or_else(|| "en_US".to_string());
        }
        language = language
            .split('.')
            .next()
            .unwrap_or("en_US")
            .split('@')
            .next()
            .unwrap_or("en_US")
            .replace("-", "_");
        if language == "C" || language == "POSIX" {
            language = "en_US".to_string();
        }
        match std::env::current_exe()
            .ok()
            .and_then(|path| path.parent().map(|p| p.to_path_buf()))
            .or_else(|| std::env::current_dir().ok())
        {
            Some(current_dir) => Translator {
                language: language.clone(),
                catalog: if language == "en_US" {
                    Catalog::empty()
                } else {
                    File::open(
                        current_dir
                            .join(&language)
                            .join("LC_MESSAGES")
                            .join(format!(
                                "{}.mo",
                                AppInfo::default().short_name().to_lowercase()
                            )),
                    )
                    .ok()
                    .and_then(|f| Catalog::parse(f).ok())
                    .or_else(|| {
                        File::open(
                            current_dir
                                .join(language.split('_').next().unwrap_or("en_US"))
                                .join("LC_MESSAGES")
                                .join(format!(
                                    "{}.mo",
                                    AppInfo::default().short_name().to_lowercase()
                                )),
                        )
                        .ok()
                        .and_then(|f| Catalog::parse(f).ok())
                    })
                    .unwrap_or_else(Catalog::empty)
                },
            },
            None => Translator {
                language: language.clone(),
                catalog: Catalog::empty(),
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
