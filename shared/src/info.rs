use semver::Version;
use std::sync::OnceLock;

#[derive(Debug, Clone, Eq, PartialEq)]
pub enum DeploymentMode {
    Local,
    Flatpak,
    Snap,
    Wsl,
}

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

pub fn app_artist_names() -> String {
    APP_ARTISTS
        .iter()
        .map(|(key, _)| *key)
        .collect::<Vec<&str>>()
        .join("\n")
}

pub fn app_designer_names() -> String {
    APP_DESIGNERS
        .iter()
        .map(|(key, _)| *key)
        .collect::<Vec<&str>>()
        .join("\n")
}

pub fn app_developer_names() -> String {
    APP_DEVELOPERS
        .iter()
        .map(|(key, _)| *key)
        .collect::<Vec<&str>>()
        .join("\n")
}

pub fn app_version() -> &'static Version {
    static VERSION: OnceLock<Version> = OnceLock::new();
    VERSION.get_or_init(|| Version::parse(&format!("{}-next", env!("CARGO_PKG_VERSION"))).unwrap())
}

pub fn deployment_mode() -> DeploymentMode {
    #[cfg(any(target_os = "windows", target_os = "macos"))]
    return DeploymentMode::Local;
    #[cfg(target_os = "linux")]
    {
        if let Ok(res) = std::fs::exists("./flatpak-info")
            && res
        {
            DeploymentMode::Flatpak
        } else if std::env::var("SNAP").is_ok() {
            DeploymentMode::Snap
        } else if let Ok(res) = std::fs::exists("/proc/sys/fs/binfmt_misc/WSLInterop")
            && res
        {
            DeploymentMode::Wsl
        } else {
            DeploymentMode::Local
        }
    }
}

pub fn is_app_portable() -> bool {
    static IS_PORTABLE: OnceLock<bool> = OnceLock::new();
    *IS_PORTABLE.get_or_init(|| std::env::args().any(|arg| arg == "--portable"))
}
