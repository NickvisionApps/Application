use crate::{
    APP_NAME, APP_REPO_NAME, APP_REPO_OWNER, Configuration, FolderBrowser, Translator, app_version,
};
use directories::BaseDirs;
use reup::{GitHubUpdater, UpdateProvider, UpdateType};
use semver::Version;
use std::path::PathBuf;
use std::sync::Arc;

#[derive(Debug)]
pub struct AppState {
    configuration: Configuration,
    folder_browser: FolderBrowser,
    translator: Arc<Translator>,
    updater: Arc<GitHubUpdater>,
}

impl AppState {
    pub fn new() -> Self {
        AppState::default()
    }

    pub fn can_close(&self) -> bool {
        true
    }

    pub fn check_for_updates(&self) -> impl Future<Output = Option<Version>> + Send + 'static {
        let updater = self.updater();
        let update_type = if self.configuration.allow_preview_updates() {
            UpdateType::Preview
        } else {
            UpdateType::Stable
        };
        async move {
            let update_res = updater.get_latest_version(update_type).await;
            update_res.ok().filter(|version| *version > *app_version())
        }
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

    pub fn install_update(&self) -> impl Future<Output = Option<PathBuf>> + Send + 'static {
        let updater = self.updater();
        let update_type = if self.configuration.allow_preview_updates() {
            UpdateType::Preview
        } else {
            UpdateType::Stable
        };
        async move {
            let path = BaseDirs::new()
                .expect("Unable to load base directories")
                .cache_dir()
                .join(APP_NAME)
                .join(updater.target_asset_name());
            let res = updater.download_update(update_type, &path).await;
            if res.is_ok() { Some(path) } else { None }
        }
    }

    pub fn translator(&self) -> Arc<Translator> {
        Arc::clone(&self.translator)
    }

    fn updater(&self) -> Arc<GitHubUpdater> {
        Arc::clone(&self.updater)
    }
}

impl Default for AppState {
    fn default() -> Self {
        let configuration = Configuration::load().unwrap();
        let folder_browser = FolderBrowser::default();
        let translator = Arc::new(Translator::new(configuration.translation_language()));
        AppState {
            configuration,
            folder_browser,
            translator,
            #[cfg(all(target_os = "windows", target_arch = "x86_64"))]
            updater: Arc::new(GitHubUpdater::new(
                APP_REPO_OWNER,
                APP_REPO_NAME,
                "NickvisionApplicationSetup.exe",
            )),
            #[cfg(all(target_os = "windows", target_arch = "aarch64"))]
            updater: Arc::new(GitHubUpdater::new(
                APP_REPO_OWNER,
                APP_REPO_NAME,
                "NickvisionApplicationSetup-arm64.exe",
            )),
            #[cfg(all(target_os = "linux", target_arch = "x86_64"))]
            updater: Arc::new(GitHubUpdater::new(
                APP_REPO_OWNER,
                APP_REPO_NAME,
                "org.nickvision.application.x64.flatpak",
            )),
            #[cfg(all(target_os = "linux", target_arch = "aarch64"))]
            updater: Arc::new(GitHubUpdater::new(
                APP_REPO_OWNER,
                APP_REPO_NAME,
                "org.nickvision.application.aarch64.flatpak",
            )),
            #[cfg(all(target_os = "macos", target_arch = "x86_64"))]
            updater: Arc::new(GitHubUpdater::new(
                APP_REPO_OWNER,
                APP_REPO_NAME,
                "Application-macOS-x64.zip",
            )),
            #[cfg(all(target_os = "macos", target_arch = "aarch64"))]
            updater: Arc::new(GitHubUpdater::new(
                APP_REPO_OWNER,
                APP_REPO_NAME,
                "Application-macOS-arm64.zip",
            )),
        }
    }
}
