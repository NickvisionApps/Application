use semver::Version;
use std::sync::OnceLock;

pub const ID: &str = "org.nickvision.application";
pub const NAME: &str = "Nickvision Application";
pub const DESCRIPTION: &str = "Create new Nickvision applications";
pub const ENGLISH_SHORT_NAME: &str = "Application";
pub const SHORT_NAME: &str = "Application";
pub const CHANGELOG: &str = "- Initial release";
pub const REPO_URL: &str = "https://github.com/NickvisionApps/Application";
pub const ISSUES_URL: &str = "https://github.com/NickvisionApps/Application/issues";
pub const DISCUSSION_URL: &str = "https://github.com/NickvisionApps/Application/discussions";
pub const DEVELOPERS: &[(&str, &str)] = &[
    ("Nicholas Logozzo", "https://github.com/nlogozzo"),
    (
        "Contributors on GitHub ❤️",
        "https://github.com/NickvisionApps/Application/graphs/contributors",
    ),
];
pub const DESIGNERS: &[(&str, &str)] = &[
    ("Nicholas Logozzo", "https://github.com/nlogozzo"),
    ("Fyodor Sobolev", "https://github.com/fysobolev"),
    ("DaPigGuy", "https://github.com/DaPigGuy"),
];
pub const ARTISTS: &[(&str, &str)] = &[("David Lapshin", "https://github.com/daudix")];

pub fn version() -> &'static Version {
    static VERSION: OnceLock<Version> = OnceLock::new();
    VERSION.get_or_init(|| Version::parse(&format!("{}-next", env!("CARGO_PKG_VERSION"))).unwrap())
}

pub fn is_portable() -> bool {
    static IS_PORTABLE: OnceLock<bool> = OnceLock::new();
    *IS_PORTABLE.get_or_init(|| std::env::args().any(|arg| arg == "--portable"))
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn version_not_empty() {
        assert!(version().major > 2025);
        assert!(version().minor > 0);
    }

    #[test]
    fn is_portable_not() {
        assert!(!is_portable());
    }
}
