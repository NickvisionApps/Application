use gettext::Catalog;
use std::fs::File;
use std::path::PathBuf;

#[derive(Debug)]
pub struct Translator {
    language: String,
    short_name: String,
    locale_dir: PathBuf,
    catalog: Catalog,
    available_languages: Vec<String>,
}

impl Translator {
    pub fn new(
        short_name: impl Into<String>,
        locale_dir: impl Into<PathBuf>,
        language: Option<impl Into<String>>,
    ) -> Self {
        let short_name = short_name.into();
        let locale_dir = locale_dir.into();
        let mut available_languages = vec!["en_US".to_string()];
        let mo_name = format!("{}.mo", short_name.to_lowercase());
        if let Ok(entries) = std::fs::read_dir(&locale_dir) {
            for entry in entries.flatten() {
                let path = entry.path();
                if path.is_dir()
                    && path.join("LC_MESSAGES").join(&mo_name).exists()
                    && let Some(name) = path.file_name().and_then(|n| n.to_str())
                {
                    available_languages.push(name.to_string());
                }
            }
        }
        available_languages.sort();
        let mut this = Self {
            language: String::default(),
            short_name,
            locale_dir,
            catalog: Catalog::empty(),
            available_languages,
        };
        if let Some(language) = language {
            this.set_language(language);
        } else {
            this.set_language(String::default());
        }
        this
    }

    pub fn language(&self) -> &str {
        &self.language
    }

    pub fn set_language(&mut self, language: impl Into<String>) {
        let mut language = language.into();
        if language.is_empty() || language == "C" {
            language = std::env::var("LC_ALL")
                .or_else(|_| std::env::var("LC_MESSAGES"))
                .or_else(|_| std::env::var("LANG"))
                .ok()
                .or_else(sys_locale::get_locale)
                .map(|lang| normalize_language(&lang))
                .unwrap_or_else(|| "en_US".to_string());
        } else {
            language = normalize_language(&language);
        }
        let mo_name = format!("{}.mo", self.short_name.to_lowercase());
        self.catalog = if language == "en_US" {
            Catalog::empty()
        } else {
            File::open(
                self.locale_dir
                    .join(&language)
                    .join("LC_MESSAGES")
                    .join(&mo_name),
            )
            .ok()
            .and_then(|f| Catalog::parse(f).ok())
            .or_else(|| {
                let base_language = language.split('_').next().unwrap_or("en_US");
                File::open(
                    self.locale_dir
                        .join(base_language)
                        .join("LC_MESSAGES")
                        .join(&mo_name),
                )
                .ok()
                .and_then(|f| Catalog::parse(f).ok())
            })
            .unwrap_or_else(Catalog::empty)
        };
        self.language = language;
    }

    pub fn available_languages(&self) -> &[String] {
        &self.available_languages
    }

    pub fn _g(&self, msgid: &str) -> String {
        self.catalog.gettext(msgid).to_string()
    }

    pub fn _f<A: AsRef<str>>(&self, msgid: &str, args: &[A]) -> String {
        format(self.catalog.gettext(msgid), args)
    }

    pub fn _n(&self, msgid: &str, msgid_plural: &str, n: impl Into<u64>) -> String {
        self.catalog
            .ngettext(msgid, msgid_plural, n.into())
            .to_string()
    }

    pub fn _nf<A: AsRef<str>>(
        &self,
        msgid: &str,
        msgid_plural: &str,
        n: impl Into<u64>,
        args: &[A],
    ) -> String {
        format(self.catalog.ngettext(msgid, msgid_plural, n.into()), args)
    }

    pub fn _p(&self, msgctxt: &str, msgid: &str) -> String {
        self.catalog.pgettext(msgctxt, msgid).to_string()
    }

    pub fn _pf<A: AsRef<str>>(&self, msgctxt: &str, msgid: &str, args: &[A]) -> String {
        format(self.catalog.pgettext(msgctxt, msgid), args)
    }

    pub fn _np(&self, msgctxt: &str, msgid: &str, msgid_plural: &str, n: impl Into<u64>) -> String {
        self.catalog
            .npgettext(msgctxt, msgid, msgid_plural, n.into())
            .to_string()
    }

    pub fn _npf<A: AsRef<str>>(
        &self,
        msgctxt: &str,
        msgid: &str,
        msgid_plural: &str,
        n: impl Into<u64>,
        args: &[A],
    ) -> String {
        format(
            self.catalog
                .npgettext(msgctxt, msgid, msgid_plural, n.into()),
            args,
        )
    }
}

fn format(s: impl Into<String>, args: &[impl AsRef<str>]) -> String {
    let mut result = s.into();
    for (i, arg) in args.iter().enumerate() {
        let placeholder = format!("{{{}}}", i);
        result = result.replace(&placeholder, arg.as_ref());
    }
    result
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
