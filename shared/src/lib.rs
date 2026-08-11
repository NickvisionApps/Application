#[cfg(not(any(target_os = "windows", target_os = "macos", target_os = "linux")))]
compile_error!("Application only supports Windows, macOS, and Linux");

mod config;
mod folder;
mod info;
mod state;
mod translation;
mod update;

pub use config::{ApplicationTheme, Configuration, WindowGeometry, WindowGeometryBuilder};
pub use folder::FolderBrowser;
pub use info::{
    APP_ARTISTS, APP_CHANGELOG, APP_DESCRIPTION, APP_DESIGNERS, APP_DEVELOPERS, APP_DISCUSSION_URL,
    APP_ENGLISH_SHORT_NAME, APP_ID, APP_ISSUES_URL, APP_NAME, APP_REPO_NAME, APP_REPO_OWNER,
    APP_REPO_URL, app_artist_names, app_designer_names, app_developer_names, app_version,
    debugging_information, is_app_portable,
};
pub use state::AppState;
pub use translation::Translator;
pub use update::Updater;
