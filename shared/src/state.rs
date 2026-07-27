use crate::{APP_REPO_NAME, APP_REPO_OWNER, Configuration, FolderBrowser, Translator};
use reup::GitHubUpdater;

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

    pub fn can_close(&self) -> bool {
        true
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
                APP_REPO_OWNER,
                APP_REPO_NAME,
                "NickvisionApplicationSetup.exe",
            ),
            #[cfg(all(target_os = "windows", target_arch = "aarch64"))]
            updater: GitHubUpdater::new(
                APP_REPO_OWNER,
                APP_REPO_NAME,
                "NickvisionApplicationSetup-arm64.exe",
            ),
            #[cfg(all(target_os = "linux", target_arch = "x86_64"))]
            updater: GitHubUpdater::new(
                APP_REPO_OWNER,
                APP_REPO_NAME,
                "org.nickvision.application.x64.flatpak",
            ),
            #[cfg(all(target_os = "linux", target_arch = "aarch64"))]
            updater: GitHubUpdater::new(
                APP_REPO_OWNER,
                APP_REPO_NAME,
                "org.nickvision.application.aarch64.flatpak",
            ),
            #[cfg(all(target_os = "macos", target_arch = "x86_64"))]
            updater: GitHubUpdater::new(APP_REPO_OWNER, APP_REPO_NAME, "Application-macOS-x64.zip"),
            #[cfg(all(target_os = "macos", target_arch = "aarch64"))]
            updater: GitHubUpdater::new(
                APP_REPO_OWNER,
                APP_REPO_NAME,
                "Application-macOS-arm64.zip",
            ),
        }
    }
}
