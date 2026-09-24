use crate::config::Configuration;
use crate::product::{DeploymentMode, ProductInfo};
use directories::BaseDirs;
use reup::{GitHubUpdater, UpdateProvider};
use semver::Version;
use std::ops::ControlFlow;
use std::sync::Mutex;
use tauri::{AppHandle, Manager, State, command};

#[command]
pub async fn check_for_updates(
    app: AppHandle,
    configuration: State<'_, Mutex<Configuration>>,
    product_info: State<'_, ProductInfo>,
) -> Result<Version, tauri::Error> {
    let update_type = configuration.lock().unwrap().update_type();
    let version = app
        .state::<GitHubUpdater>()
        .get_latest_version(update_type)
        .await
        .ok()
        .filter(|version| version > product_info.version());
    if let Some(version) = version {
        Ok(version)
    } else {
        Err(tauri::Error::AssetNotFound("No updates available".into()))
    }
}

#[command]
pub async fn install_update(
    app: AppHandle,
    configuration: State<'_, Mutex<Configuration>>,
    product_info: State<'_, ProductInfo>,
    updater: State<'_, GitHubUpdater>,
) -> Result<(), tauri::Error> {
    if ProductInfo::deployment_mode() != DeploymentMode::Local {
        return Err(tauri::Error::Io(std::io::Error::other(
            "Unable to install update on non-local installations",
        )));
    }
    let path = BaseDirs::new()
        .ok_or_else(|| tauri::Error::Io(std::io::Error::other("Unable to load base directories")))?
        .cache_dir()
        .join(product_info.name())
        .join(updater.target_asset_name());
    let update_type = configuration.lock().unwrap().update_type();
    std::fs::create_dir_all(
        path.parent().ok_or_else(|| {
            tauri::Error::Io(std::io::Error::other("Download path has no parent"))
        })?,
    )?;
    app.state::<GitHubUpdater>()
        .download_update(update_type, &path, |_, _| ControlFlow::Continue(()))
        .await
        .map_err(|e| tauri::Error::Io(std::io::Error::other(e.to_string())))?;
    #[cfg(target_os = "windows")]
    {
        let status = std::process::Command::new(&path).status()?;
        std::fs::remove_file(&path)?;
        if !status.success() {
            Err(tauri::Error::Io(std::io::Error::other(
                "Unable to start installer",
            )))
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
        std::fs::remove_file(&path)?;
        if !status.success() {
            return Err(tauri::Error::Io(std::io::Error::other(
                "Failed to extract update archive",
            )));
        }
        let new_app = std::fs::read_dir(&extract_dir)?
            .filter_map(Result::ok)
            .map(|entry| entry.path())
            .find(|p| p.extension().is_some_and(|ext| ext == "app"))
            .ok_or_else(|| {
                tauri::Error::Io(std::io::Error::other(
                    "No .app bundle found in update archive",
                ))
            })?;
        std::process::Command::new("xattr")
            .arg("-dr")
            .arg("com.apple.quarantine")
            .arg(&new_app)
            .status()?;
        let current_exe = std::env::current_exe()?;
        let current_app = current_exe
            .ancestors()
            .find(|p| p.extension().is_some_and(|ext| ext == "app"))
            .ok_or_else(|| {
                tauri::Error::Io(std::io::Error::other(
                    "Unable to determine running app bundle",
                ))
            })?;
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
        std::fs::remove_dir_all(&extract_dir)?;
        std::process::Command::new(current_app.join("Contents/MacOS").join(
            current_exe.file_name().ok_or_else(|| {
                tauri::Error::Io(std::io::Error::other("Unable to get exe file name"))
            })?,
        ))
        .spawn()?;
        std::process::exit(0);
    }
}
