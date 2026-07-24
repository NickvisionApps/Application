#[cfg(not(any(target_os = "windows", target_os = "macos", target_os = "linux")))]
compile_error!("armoire only supports Windows, macOS, and Linux");

use reup::GitHubUpdater;

pub mod app_info;
mod config;
mod folder;
mod translation;

pub use config::{ApplicationTheme, Configuration, WindowGeometry, WindowGeometryBuilder};
pub use folder::FolderBrowser;
pub use translation::Translator;

#[derive(Debug)]
pub struct AppState {
    configuration: Configuration,
    folder_browser: FolderBrowser,
    translator: Translator,
    updater: GitHubUpdater,
}

impl AppState {
    pub fn new() -> Self {
        AppState::default()
    }

    pub fn configuration(&self) -> &Configuration {
        &self.configuration
    }

    pub fn configuration_mut(&mut self) -> &mut Configuration {
        &mut self.configuration
    }

    pub fn folder_browser(&self) -> &FolderBrowser {
        &self.folder_browser
    }

    pub fn folder_browser_mut(&mut self) -> &mut FolderBrowser {
        &mut self.folder_browser
    }

    pub fn translator(&self) -> &Translator {
        &self.translator
    }

    pub fn updater(&self) -> &GitHubUpdater {
        &self.updater
    }
}

impl Default for AppState {
    fn default() -> Self {
        let configuration = Configuration::load().unwrap();
        let folder_browser = FolderBrowser::default();
        let translator = Translator::new(configuration.translation_language());
        AppState {
            configuration,
            folder_browser,
            translator,
            #[cfg(all(target_os = "windows", target_arch = "x86_64"))]
            updater: GitHubUpdater::new(
                app_info::REPO_OWNER,
                app_info::REPO_NAME,
                "NickvisionApplicationSetup.exe",
            ),
            #[cfg(all(target_os = "windows", target_arch = "aarch64"))]
            updater: GitHubUpdater::new(
                app_info::REPO_OWNER,
                app_info::REPO_NAME,
                "NickvisionApplicationSetup-arm64.exe",
            ),
            #[cfg(all(target_os = "linux", target_arch = "x86_64"))]
            updater: GitHubUpdater::new(
                app_info::REPO_OWNER,
                app_info::REPO_NAME,
                "org.nickvision.application.x64.flatpak",
            ),
            #[cfg(all(target_os = "linux", target_arch = "aarch64"))]
            updater: GitHubUpdater::new(
                app_info::REPO_OWNER,
                app_info::REPO_NAME,
                "org.nickvision.application.aarch64.flatpak",
            ),
            #[cfg(all(target_os = "macos", target_arch = "x86_64"))]
            updater: GitHubUpdater::new(
                app_info::REPO_OWNER,
                app_info::REPO_NAME,
                "Application-macOS-x64.zip",
            ),
            #[cfg(all(target_os = "macos", target_arch = "aarch64"))]
            updater: GitHubUpdater::new(
                app_info::REPO_OWNER,
                app_info::REPO_NAME,
                "Application-macOS-arm64.zip",
            ),
        }
    }
}
