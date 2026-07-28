use crate::{APP_NAME, APP_REPO_NAME, APP_REPO_OWNER, app_version};
use directories::BaseDirs;
use reup::{GitHubUpdater, UpdateProvider, UpdateType};
use semver::Version;
use std::path::PathBuf;

#[derive(Debug, Clone)]
pub struct Updater {
    update_type: UpdateType,
    updater: GitHubUpdater,
}

impl Updater {
    pub fn new(prefer_preview: bool) -> Self {
        Updater {
            update_type: if prefer_preview {
                UpdateType::Preview
            } else {
                UpdateType::Stable
            },
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

    pub async fn check_for_updates(&self) -> Option<Version> {
        self.updater
            .get_latest_version(self.update_type)
            .await
            .ok()
            .filter(|version| *version > *app_version())
    }

    pub async fn install_update(&self, on_progress: impl Fn(u64, u64) + Send) -> Option<PathBuf> {
        let path = BaseDirs::new()
            .expect("Unable to load base directories")
            .cache_dir()
            .join(APP_NAME)
            .join(self.updater.target_asset_name());
        let res = self
            .updater
            .download_update(self.update_type, &path, on_progress)
            .await;
        if res.is_ok() { Some(path) } else { None }
    }
}
