use crate::{
    config::Configuration, folder::FolderBrowser, info, info::DeploymentMode, translation,
};
use chrono::{Local, Timelike};
use directories::BaseDirs;
use reup::{GitHubUpdater, UpdateProvider, UpdateType};
use semver::Version;
use std::ops::{Deref, DerefMut};

#[derive(Debug, Clone)]
pub struct AppController {
    configuration: Configuration,
    folder_browser: FolderBrowser,
    updater: GitHubUpdater,
}

impl AppController {
    pub fn new() -> Self {
        AppController::default()
    }

    pub fn can_close(&self) -> bool {
        true
    }

    pub async fn check_for_updates(&self) -> Option<Version> {
        self.updater
            .get_latest_version(self.update_type())
            .await
            .ok()
            .filter(|version| *version > *info::app_version())
    }

    pub fn folder_browser(&self) -> &FolderBrowser {
        &self.folder_browser
    }

    pub fn folder_browser_mut(&mut self) -> &mut FolderBrowser {
        &mut self.folder_browser
    }

    pub fn greeting(&self) -> String {
        let hours = Local::now().hour();
        if hours < 6 {
            translation::_p("Night", "Good Morning!")
        } else if hours < 12 {
            translation::_p("Morning", "Good Morning!")
        } else if hours < 18 {
            translation::_g("Good Afternoon!")
        } else {
            translation::_g("Good Evening!")
        }
    }

    pub async fn install_update(
        &self,
        on_progress: impl Fn(u64, u64) + Send,
    ) -> Result<(), Box<dyn std::error::Error>> {
        if info::deployment_mode() != DeploymentMode::Local {
            return Err("Unable to install update on non-local installations".into());
        }
        let path = BaseDirs::new()
            .expect("Unable to load base directories")
            .cache_dir()
            .join(info::APP_NAME)
            .join(self.updater.target_asset_name());
        self.updater
            .download_update(self.update_type(), &path, on_progress)
            .await?;
        #[cfg(target_os = "windows")]
        {
            let status = std::process::Command::new(&path).status()?;
            if !status.success() {
                Err("Unable to start installer".into())
            } else {
                Ok(())
            }
        }
        #[cfg(target_os = "linux")]
        return Ok(());
        #[cfg(target_os = "macos")]
        {
            let extract_dir = path.with_extension("");
            if extract_dir.exists() {
                std::fs::remove_dir_all(&extract_dir)?;
            }
            std::fs::create_dir_all(&extract_dir)?;
            let status = std::process::Command::new("ditto")
                .arg("-xk")
                .arg(&path)
                .arg(&extract_dir)
                .status()?;
            if !status.success() {
                return Err("Failed to extract update archive".into());
            }
            let new_app = std::fs::read_dir(&extract_dir)?
                .filter_map(Result::ok)
                .map(|entry| entry.path())
                .find(|p| p.extension().is_some_and(|ext| ext == "app"))
                .ok_or("No .app bundle found in update archive")?;
            let current_exe = std::env::current_exe()?;
            let current_app = current_exe
                .ancestors()
                .find(|p| p.extension().is_some_and(|ext| ext == "app"))
                .ok_or("Unable to determine running app bundle")?;
            let backup_app = current_app.with_extension("app.bak");
            if backup_app.exists() {
                std::fs::remove_dir_all(&backup_app)?;
            }
            std::fs::rename(current_app, &backup_app)?;
            if let Err(error) = std::fs::rename(&new_app, current_app) {
                std::fs::rename(&backup_app, current_app)?;
                return Err(error.into());
            }
            std::fs::remove_dir_all(&backup_app)?;
            std::process::Command::new(
                current_app.join("Contents/MacOS").join(
                    current_exe
                        .file_name()
                        .ok_or("Unable to get exe file name")?,
                ),
            )
            .spawn()?;
            std::process::exit(0);
        }
    }

    fn update_type(&self) -> UpdateType {
        if self.configuration.allow_preview_updates() {
            UpdateType::Preview
        } else {
            UpdateType::Stable
        }
    }
}

impl Default for AppController {
    fn default() -> Self {
        let configuration = Configuration::load().unwrap_or_else(|_| Configuration::default());
        let folder_browser = FolderBrowser::default();
        translation::init(configuration.translation_language());
        AppController {
            configuration,
            folder_browser,
            #[cfg(all(target_os = "windows", target_arch = "x86_64"))]
            updater: GitHubUpdater::new(
                info::APP_REPO_OWNER,
                info::APP_REPO_NAME,
                "NickvisionApplicationSetup.exe",
            ),
            #[cfg(all(target_os = "windows", target_arch = "aarch64"))]
            updater: GitHubUpdater::new(
                info::APP_REPO_OWNER,
                info::APP_REPO_NAME,
                "NickvisionApplicationSetup-arm64.exe",
            ),
            #[cfg(all(target_os = "linux", target_arch = "x86_64"))]
            updater: GitHubUpdater::new(
                info::APP_REPO_OWNER,
                info::APP_REPO_NAME,
                "org.nickvision.application.x64.flatpak",
            ),
            #[cfg(all(target_os = "linux", target_arch = "aarch64"))]
            updater: GitHubUpdater::new(
                info::APP_REPO_OWNER,
                info::APP_REPO_NAME,
                "org.nickvision.application.aarch64.flatpak",
            ),
            #[cfg(all(target_os = "macos", target_arch = "x86_64"))]
            updater: GitHubUpdater::new(APP_REPO_OWNER, APP_REPO_NAME, "Application-macOS-x64.zip"),
            #[cfg(all(target_os = "macos", target_arch = "aarch64"))]
            updater: GitHubUpdater::new(
                info::APP_REPO_OWNER,
                info::APP_REPO_NAME,
                "Application-macOS-arm64.zip",
            ),
        }
    }
}

impl Deref for AppController {
    type Target = Configuration;

    fn deref(&self) -> &Self::Target {
        &self.configuration
    }
}

impl DerefMut for AppController {
    fn deref_mut(&mut self) -> &mut Self::Target {
        &mut self.configuration
    }
}

impl AsRef<Configuration> for AppController {
    fn as_ref(&self) -> &Configuration {
        &self.configuration
    }
}

impl AsRef<FolderBrowser> for AppController {
    fn as_ref(&self) -> &FolderBrowser {
        &self.folder_browser
    }
}
