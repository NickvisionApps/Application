use crate::folder::FolderView;
use std::path::PathBuf;
use tauri::{AppHandle, Manager, command};
use tauri_plugin_dialog::DialogExt;

#[command]
pub fn close_folder() -> FolderView {
    FolderView::default()
}

#[command]
pub async fn open_folder(
    app: AppHandle,
    path: Option<PathBuf>,
) -> Result<FolderView, tauri::Error> {
    let path = if let Some(path) = path {
        if !path.is_dir() {
            return Err(tauri::Error::Io(std::io::Error::new(
                std::io::ErrorKind::NotADirectory,
                "Dropped item is not a folder",
            )));
        }
        path
    } else {
        let handle = app.clone();
        let folder = tauri::async_runtime::spawn_blocking(move || {
            handle.dialog().file().blocking_pick_folder()
        })
        .await?;
        if let Some(folder) = folder {
            folder
                .into_path()
                .map_err(|_| tauri::Error::Io(std::io::Error::other("Cannot convert to path")))?
        } else {
            return Ok(FolderView::default());
        }
    };
    app.asset_protocol_scope().allow_directory(&path, false)?;
    FolderView::new(path).map_err(tauri::Error::Io)
}
