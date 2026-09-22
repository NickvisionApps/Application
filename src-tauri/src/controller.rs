use crate::config::Configuration;
use crate::folder::FolderView;
use crate::product::{DeploymentMode, ProductInfo};
use directories::BaseDirs;
use reup::{GitHubUpdater, UpdateProvider, UpdateType};
use semver::Version;
use std::ops::{ControlFlow, Deref, DerefMut};
use std::path::PathBuf;

#[derive(Debug, Clone)]
pub struct AppController {
    product: ProductInfo,
    configuration: Configuration,
    folder_view: Option<FolderView>,
    updater: GitHubUpdater,
    can_close: bool,
}

impl AppController {
    pub fn new() -> Self {
        AppController::default()
    }

    pub fn can_close(&self) -> bool {
        self.can_close
    }

    pub fn confirm_close(&mut self) {
        self.can_close = true;
    }

    pub fn check_for_updates(&self) -> Option<Version> {
        self.updater
            .get_latest_version(self.update_type())
            .ok()
            .filter(|version| *version > *self.product.version())
    }

    pub fn close_folder(&mut self) {
        self.folder_view = None;
    }

    pub fn install_update(
        &self,
        on_progress: impl Fn(u64, u64) -> ControlFlow<()>,
    ) -> Result<(), Box<dyn std::error::Error>> {
        if ProductInfo::deployment_mode() != DeploymentMode::Local {
            return Err("Unable to install update on non-local installations".into());
        }
        let path = BaseDirs::new()
            .expect("Unable to load base directories")
            .cache_dir()
            .join(self.product.name())
            .join(self.updater.target_asset_name());
        std::fs::create_dir_all(path.parent().expect("Download path has no parent"))?;
        self.updater
            .download_update(self.update_type(), &path, on_progress)?;
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

    pub fn open_folder(&mut self, path: impl Into<PathBuf>) -> Result<FolderView, std::io::Error> {
        let folder_view = FolderView::new(path)?;
        self.folder_view = Some(folder_view.clone());
        Ok(folder_view)
    }

    pub fn product_info(&self) -> &ProductInfo {
        &self.product
    }

    pub fn reset_close(&mut self) {
        self.can_close = false;
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
        let product = ProductInfo::builder()
            .id("org.nickvision.application")
            .name("Nickvision Application")
            .short_name("Application")
            .repo_owner("NickvisionApps")
            .repo_name("Application")
            .version(Version::parse(&format!("{}-next", env!("CARGO_PKG_VERSION"))).unwrap())
            .build()
            .unwrap();
        let configuration = Configuration::load(product.name()).unwrap_or_default();
        #[cfg(all(target_os = "windows", target_arch = "x86_64"))]
        let updater = GitHubUpdater::new(
            product.repo_owner(),
            product.repo_name(),
            "NickvisionApplicationSetup.exe",
        );
        #[cfg(all(target_os = "windows", target_arch = "aarch64"))]
        let updater = GitHubUpdater::new(
            product.repo_owner(),
            product.repo_name(),
            "NickvisionApplicationSetup-arm64.exe",
        );
        #[cfg(all(target_os = "linux", target_arch = "x86_64"))]
        let updater = GitHubUpdater::new(
            product.repo_owner(),
            product.repo_name(),
            "org.nickvision.application.x64.flatpak",
        );
        #[cfg(all(target_os = "linux", target_arch = "aarch64"))]
        let updater = GitHubUpdater::new(
            product.repo_owner(),
            product.repo_name(),
            "org.nickvision.application.aarch64.flatpak",
        );
        #[cfg(all(target_os = "macos", target_arch = "x86_64"))]
        let updater = GitHubUpdater::new(
            product.repo_owner(),
            product.repo_name(),
            "Application-macOS-x64.zip",
        );
        #[cfg(all(target_os = "macos", target_arch = "aarch64"))]
        let updater = GitHubUpdater::new(
            product.repo_owner(),
            product.repo_name(),
            "Application-macOS-arm64.zip",
        );
        AppController {
            product,
            configuration,
            folder_view: None,
            updater,
            can_close: false,
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
