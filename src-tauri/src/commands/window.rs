use crate::close::CloseManager;
use crate::translation::Translator;
use std::sync::Mutex;
use tauri::{State, WebviewWindow, command};

#[command]
pub fn can_window_close(close_manager: State<'_, Mutex<CloseManager>>) -> bool {
    close_manager.lock().unwrap().can_close()
}

#[command]
pub fn confirm_window_close(close_manager: State<'_, Mutex<CloseManager>>) {
    close_manager.lock().unwrap().confirm_close();
}

#[command]
pub fn get_linux_button_layout() -> Option<String> {
    #[cfg(target_os = "linux")]
    {
        crate::window::gnome_button_layout().or_else(|| crate::window::kde_button_layout().ok())
    }
    #[cfg(not(target_os = "linux"))]
    None
}

#[command]
pub async fn show_main_window(
    window: WebviewWindow,
    translator: State<'_, Mutex<Translator>>,
) -> Result<(), tauri::Error> {
    window.set_title(&translator.lock().unwrap()._p("AppName", "Application"))?;
    window.show()?;
    window.set_focus()?;
    Ok(())
}
