use crate::close::CloseManager;
use crate::translation::Translator;
use std::sync::Mutex;
use tauri::{State, WebviewWindow, command};
use tauri_plugin_decoration::WebviewWindowExt;

#[command]
pub fn can_window_close(close_manager: State<'_, Mutex<CloseManager>>) -> bool {
    close_manager.lock().unwrap().can_close()
}

#[command]
pub fn confirm_window_close(close_manager: State<'_, Mutex<CloseManager>>) {
    close_manager.lock().unwrap().confirm_close();
}

#[command]
pub async fn show_main_window(
    window: WebviewWindow,
    translator: State<'_, Mutex<Translator>>,
) -> Result<(), tauri::Error> {
    window.set_title(&translator.lock().unwrap()._p("AppName", "Application"))?;
    if let Err(error) = window.activate_decoration().await {
        eprintln!("decoration activation failed: {error}");
        return restore_and_show(&window).await;
    }
    #[cfg(target_os = "macos")]
    if window.set_traffic_lights_inset(16.0, 26.0).await.is_err() {
        return restore_and_show(&window).await;
    }
    window.show()?;
    window.set_focus()?;
    Ok(())
}

async fn restore_and_show(window: &WebviewWindow) -> Result<(), tauri::Error> {
    window
        .restore_decoration()
        .await
        .map_err(|error| tauri::Error::Io(std::io::Error::other(error.to_string())))?;
    window.show()?;
    window.set_focus()?;
    Ok(())
}
