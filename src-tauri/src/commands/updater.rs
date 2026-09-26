use crate::config::Configuration;
use crate::product::{DeploymentMode, ProductInfo};
use semver::Version;
use serde::{Deserialize, Serialize};
use std::sync::Mutex;
use tauri::{AppHandle, Emitter, State, command};
use tauri_plugin_updater::{Update, UpdaterExt};
use url::Url;

#[derive(Debug, Clone, PartialEq, Eq, Serialize, Deserialize)]
#[serde(rename_all = "camelCase")]
pub struct UpdateDownloadProgress {
    downloaded: usize,
    total: u64,
}

impl UpdateDownloadProgress {
    pub fn new(downloaded: usize, total: u64) -> Self {
        Self { downloaded, total }
    }

    pub fn downloaded(&self) -> usize {
        self.downloaded
    }

    pub fn total(&self) -> u64 {
        self.total
    }
}

#[command]
pub async fn check_for_updates(
    app: AppHandle,
    configuration: State<'_, Mutex<Configuration>>,
    product_info: State<'_, ProductInfo>,
    pending_update: State<'_, Mutex<Option<Update>>>,
) -> Result<String, tauri::Error> {
    if let Some(update) = pending_update.lock().unwrap().clone() {
        return Ok(update.version);
    }
    let endpoint = if configuration.lock().unwrap().allow_preview_updates() {
        format!(
            "https://github.com/{}/{}/releases/download/preview/latest.json",
            product_info.repo_owner(),
            product_info.repo_name()
        )
    } else {
        format!(
            "https://github.com/{}/{}/releases/latest/download/latest.json",
            product_info.repo_owner(),
            product_info.repo_name()
        )
    }
    .parse::<Url>()
    .map_err(|e| tauri::Error::Io(std::io::Error::other(e.to_string())))?;
    let update = app
        .updater_builder()
        .endpoints(vec![endpoint])
        .map_err(|e| tauri::Error::Io(std::io::Error::other(e.to_string())))?
        .build()
        .map_err(|e| tauri::Error::Io(std::io::Error::other(e.to_string())))?
        .check()
        .await
        .map_err(|e| tauri::Error::Io(std::io::Error::other(e.to_string())))?;
    match update {
        Some(update) => {
            let version = update.version.clone();
            *pending_update.lock().unwrap() = Some(update);
            if let Ok(version) = Version::parse(&version)
                && &version > product_info.version()
            {
                Ok(version.to_string())
            } else {
                Err(tauri::Error::AssetNotFound("No updates available".into()))
            }
        }
        None => Err(tauri::Error::AssetNotFound("No updates available".into())),
    }
}

#[command]
pub async fn install_update(
    app: AppHandle,
    pending_update: State<'_, Mutex<Option<Update>>>,
) -> Result<(), tauri::Error> {
    let can_self_update = match ProductInfo::deployment_mode() {
        DeploymentMode::AppImage => true,
        #[cfg(any(target_os = "windows", target_os = "macos"))]
        DeploymentMode::Local => true,
        _ => false,
    };
    if !can_self_update {
        return Err(tauri::Error::Io(std::io::Error::other(
            "Unable to install update on non-local installations",
        )));
    }
    let update =
        pending_update.lock().unwrap().clone().ok_or_else(|| {
            tauri::Error::Io(std::io::Error::other("No update available to install"))
        })?;
    update
        .download_and_install(
            |downloaded, total| {
                let _ = app.emit(
                    "update-download-progress",
                    UpdateDownloadProgress::new(downloaded, total.unwrap_or_default()),
                );
            },
            || {
                let _ = app.emit(
                    "update-download-progress",
                    UpdateDownloadProgress::new(100, 100),
                );
            },
        )
        .await
        .map_err(|e| tauri::Error::Io(std::io::Error::other(e.to_string())))?;
    // Windows' installer relaunches the app itself and exits the current process;
    // macOS/Linux (AppImage) need an explicit restart to run the newly installed version.
    #[cfg(any(target_os = "macos", target_os = "linux"))]
    app.restart();
    #[cfg(target_os = "windows")]
    Ok(())
}
