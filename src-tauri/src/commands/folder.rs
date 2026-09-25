use crate::folder::FolderView;
use tauri::{AppHandle, Manager, command};
use tauri_plugin_dialog::DialogExt;

#[command]
pub fn close_folder() -> FolderView {
    FolderView::default()
}

#[command]
pub async fn open_folder(app: AppHandle) -> Result<FolderView, tauri::Error> {
    let handle = app.clone();
    let folder =
        tauri::async_runtime::spawn_blocking(move || handle.dialog().file().blocking_pick_folder())
            .await?;
    if let Some(folder) = folder {
        let path = folder
            .into_path()
            .map_err(|_| tauri::Error::Io(std::io::Error::other("Cannot convert to path")))?;
        app.asset_protocol_scope().allow_directory(&path, false)?;
        FolderView::new(path).map_err(tauri::Error::Io)
    } else {
        Err(tauri::Error::Io(std::io::Error::new(
            std::io::ErrorKind::InvalidFilename,
            "No file chosen",
        )))
    }
}
