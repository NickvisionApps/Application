use semver::Version;
use std::sync::OnceLock;

pub const APP_ID: &str = "org.nickvision.application";
pub const APP_NAME: &str = "Nickvision Application";
pub const APP_DESCRIPTION: &str = "Create new Nickvision applications";
pub const APP_ENGLISH_SHORT_NAME: &str = "Application";
pub const APP_CHANGELOG: &str = "- Initial release";
pub const APP_REPO_OWNER: &str = "NickvisionApps";
pub const APP_REPO_NAME: &str = "Application";
pub const APP_REPO_URL: &str = "https://github.com/NickvisionApps/Application";
pub const APP_ISSUES_URL: &str = "https://github.com/NickvisionApps/Application/issues";
pub const APP_DISCUSSION_URL: &str = "https://github.com/NickvisionApps/Application/discussions";
pub const APP_DEVELOPERS: &[(&str, &str)] = &[
    ("Nicholas Logozzo", "https://github.com/nlogozzo"),
    (
        "Contributors on GitHub ❤️",
        "https://github.com/NickvisionApps/Application/graphs/contributors",
    ),
];
pub const APP_DESIGNERS: &[(&str, &str)] = &[
    ("Nicholas Logozzo", "https://github.com/nlogozzo"),
    ("Fyodor Sobolev", "https://github.com/fysobolev"),
    ("DaPigGuy", "https://github.com/DaPigGuy"),
];
pub const APP_ARTISTS: &[(&str, &str)] = &[("David Lapshin", "https://github.com/daudix")];

pub fn app_version() -> &'static Version {
    static VERSION: OnceLock<Version> = OnceLock::new();
    VERSION.get_or_init(|| Version::parse(&format!("{}-next", env!("CARGO_PKG_VERSION"))).unwrap())
}

pub fn is_app_portable() -> bool {
    static IS_PORTABLE: OnceLock<bool> = OnceLock::new();
    *IS_PORTABLE.get_or_init(|| std::env::args().any(|arg| arg == "--portable"))
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn version_not_empty() {
        assert!(app_version().major > 2025);
        assert!(app_version().minor > 0);
    }

    #[test]
    fn is_portable_not() {
        assert!(!is_app_portable());
    }
}
