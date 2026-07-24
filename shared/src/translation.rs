use crate::app_info;
use gettext::Catalog;
use std::fs::File;

#[derive(Debug)]
pub struct Translator {
    language: String,
    catalog: Catalog,
}

impl Translator {
    pub fn new<T: Into<String>>(language: T) -> Self {
        let mut language = language.into();
        if language.is_empty() || language == "C" {
            language = Self::detect_language();
        } else {
            language = Self::normalize_language(&language);
        }
        let mo_name = format!("{}.mo", app_info::ENGLISH_SHORT_NAME.to_lowercase());
        Translator {
            language: language.clone(),
            catalog: match std::env::current_exe()
                .ok()
                .and_then(|path| path.parent().map(|p| p.to_path_buf()))
                .or_else(|| std::env::current_dir().ok())
            {
                Some(current_dir) => {
                    if language == "en_US" {
                        Catalog::empty()
                    } else {
                        File::open(
                            current_dir
                                .join(&language)
                                .join("LC_MESSAGES")
                                .join(&mo_name),
                        )
                        .ok()
                        .and_then(|f| Catalog::parse(f).ok())
                        .or_else(|| {
                            let base_language = language.split('_').next().unwrap_or("en_US");
                            File::open(
                                current_dir
                                    .join(base_language)
                                    .join("LC_MESSAGES")
                                    .join(&mo_name),
                            )
                            .ok()
                            .and_then(|f| Catalog::parse(f).ok())
                        })
                        .unwrap_or_else(Catalog::empty)
                    }
                }
                None => Catalog::empty(),
            },
        }
    }

    pub fn available_languages() -> Vec<String> {
        let mut languages = vec![];
        let mo_name = format!("{}.mo", app_info::ENGLISH_SHORT_NAME.to_lowercase());
        if let Some(current_dir) = std::env::current_exe()
            .ok()
            .and_then(|path| path.parent().map(|p| p.to_path_buf()))
            .or_else(|| std::env::current_dir().ok())
            && let Ok(entries) = std::fs::read_dir(current_dir)
        {
            for entry in entries.flatten() {
                let path = entry.path();
                if path.is_dir()
                    && path.join("LC_MESSAGES").join(&mo_name).exists()
                    && let Some(name) = path.file_name().and_then(|n| n.to_str())
                {
                    languages.push(name.to_string());
                }
            }
        }
        languages.sort();
        languages
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

    fn detect_language() -> String {
        std::env::var("LC_ALL")
            .or_else(|_| std::env::var("LC_MESSAGES"))
            .or_else(|_| std::env::var("LANG"))
            .ok()
            .or_else(sys_locale::get_locale)
            .map(|lang| Self::normalize_language(&lang))
            .unwrap_or_else(|| "en_US".to_string())
    }

    fn normalize_language(language: &str) -> String {
        let normalized = language
            .split('.')
            .next()
            .unwrap_or("en_US")
            .split('@')
            .next()
            .unwrap_or("en_US")
            .replace('-', "_");
        if normalized.is_empty() || normalized == "C" || normalized == "POSIX" {
            "en_US".to_string()
        } else {
            normalized
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn language_normalization() {
        assert_eq!(Translator::normalize_language("pt-BR.UTF-8@foo"), "pt_BR");
        assert_eq!(Translator::normalize_language("ru_RU.UTF-8"), "ru_RU");
        assert_eq!(Translator::normalize_language("C"), "en_US");
        assert_eq!(Translator::normalize_language("POSIX"), "en_US");
        assert_eq!(Translator::normalize_language("C.UTF-8"), "en_US");
    }

    #[test]
    fn language_detection() {
        let language = Translator::detect_language();
        assert!(!language.is_empty());
        assert_ne!(language, "C");
    }

    #[test]
    fn translator_language_normalization() {
        let translator = Translator::new("fr-CA.UTF-8@foo");
        assert_eq!(translator.language(), "fr_CA");
    }

    #[test]
    fn translator_missing_catalog() {
        let translator = Translator::new("zz_ZZ");
        assert_eq!(translator.language(), "zz_ZZ");
        assert_eq!(translator._g("Hello"), "Hello");
        assert_eq!(translator._f("Hello {0}", &["World"]), "Hello World");
    }
}
