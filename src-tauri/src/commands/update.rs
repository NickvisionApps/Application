use crate::config::Configuration;
use crate::product::{DeploymentMode, ProductInfo};
use crate::update::{UpdateDownloadProgress, UpdateInformation, latest_update};
use std::sync::Mutex;
use tauri::{AppHandle, Emitter, State, command};

#[command]
pub async fn get_new_update(
    app: AppHandle,
    configuration: State<'_, Mutex<Configuration>>,
    product_info: State<'_, ProductInfo>,
) -> Result<Option<UpdateInformation>, tauri::Error> {
    let allow_preview = configuration.lock().unwrap().allow_preview_updates();
    if let Some(update) = latest_update(&app, &product_info, allow_preview)
        .await
        .map_err(|e| tauri::Error::Io(std::io::Error::other(e.to_string())))?
    {
        Ok(Some(UpdateInformation::new(&update)))
    } else {
        Ok(None)
    }
}

#[command]
pub async fn install_update(
    app: AppHandle,
    configuration: State<'_, Mutex<Configuration>>,
    product_info: State<'_, ProductInfo>,
) -> Result<(), tauri::Error> {
    let allow_preview = configuration.lock().unwrap().allow_preview_updates();
    if let Some(update) = latest_update(&app, &product_info, allow_preview)
        .await
        .map_err(|e| tauri::Error::Io(std::io::Error::other(e.to_string())))?
    {
        #[cfg(any(target_os = "windows", target_os = "macos"))]
        if ProductInfo::deployment_mode() != DeploymentMode::Local {
            return Err(tauri::Error::Io(std::io::Error::other(
                "Unable to install update on non-local installations",
            )));
        }
        #[cfg(target_os = "linux")]
        if ProductInfo::deployment_mode() != DeploymentMode::AppImage {
            return Err(tauri::Error::Io(std::io::Error::other(
                "Unable to install update on non-AppImage installations",
            )));
        }
        let mut downloaded = 0;
        let emit_error = Mutex::new(None);
        update
            .download_and_install(
                |chunk, total| {
                    downloaded += chunk;
                    if let Err(e) = app.emit(
                        "update-download-progress",
                        UpdateDownloadProgress::new(downloaded, total.unwrap_or_default()),
                    ) {
                        *emit_error.lock().unwrap() = Some(e);
                    }
                },
                || {
                    if let Err(e) = app.emit(
                        "update-download-progress",
                        UpdateDownloadProgress::new(100, 100),
                    ) {
                        *emit_error.lock().unwrap() = Some(e);
                    }
                },
            )
            .await
            .map_err(|e| tauri::Error::Io(std::io::Error::other(e.to_string())))?;
        if let Some(e) = emit_error.into_inner().unwrap() {
            return Err(e);
        }
        #[cfg(any(target_os = "macos", target_os = "linux"))]
        app.restart();
    }
    Ok(())
}
