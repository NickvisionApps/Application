use crate::{
    APP_NAME, APP_REPO_NAME, APP_REPO_OWNER, app_version, info::DeploymentMode,
    info::deployment_mode,
};
use directories::BaseDirs;
use reup::{GitHubUpdater, UpdateProvider, UpdateType};
use semver::Version;

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

    pub async fn install_update(
        &self,
        on_progress: impl Fn(u64, u64) + Send,
    ) -> Result<(), Box<dyn std::error::Error>> {
        if deployment_mode() != DeploymentMode::Local {
            return Err("Unable to install update on non-local installations".into());
        }
        let path = BaseDirs::new()
            .expect("Unable to load base directories")
            .cache_dir()
            .join(APP_NAME)
            .join(self.updater.target_asset_name());
        self.updater
            .download_update(self.update_type, &path, on_progress)
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
}
