use crate::close::CloseManager;
use crate::translation::Translator;
use std::sync::Mutex;
use tauri::{State, WebviewWindow, command};

#[command]
pub fn can_window_close(close_manager: State<'_, Mutex<CloseManager>>) -> bool {
    close_manager.lock().unwrap().can_close()
}

#[command]
pub fn clear_windows_snap_geometry(window: WebviewWindow) -> Result<(), String> {
    #[cfg(target_os = "windows")]
    {
        crate::platform::windows::uninstall_snap_layout(&window)
    }
    #[cfg(not(target_os = "windows"))]
    {
        let _ = window;
        Ok(())
    }
}

#[command]
pub fn confirm_window_close(close_manager: State<'_, Mutex<CloseManager>>) {
    close_manager.lock().unwrap().confirm_close();
}

#[command]
pub fn get_linux_button_layout() -> Option<String> {
    #[cfg(target_os = "linux")]
    {
        crate::platform::linux::button_layout()
    }
    #[cfg(not(target_os = "linux"))]
    None
}

#[command]
pub fn get_linux_titlebar_icons(
    window: WebviewWindow,
) -> Option<std::collections::BTreeMap<&'static str, String>> {
    #[cfg(target_os = "linux")]
    {
        crate::platform::linux::titlebar_icons(&window)
    }
    #[cfg(not(target_os = "linux"))]
    {
        let _ = window;
        None
    }
}

#[command]
pub fn show_main_window(
    window: WebviewWindow,
    translator: State<'_, Mutex<Translator>>,
) -> Result<(), tauri::Error> {
    window.set_title(&translator.lock().unwrap()._p("AppName", "Application"))?;
    window.show()?;
    window.set_focus()?;
    Ok(())
}

#[command]
pub fn update_windows_snap_geometry(
    window: WebviewWindow,
    x: i32,
    y: i32,
    width: u32,
    height: u32,
    titlebar_height: u32,
    control_band_width: u32,
) -> Result<(), String> {
    #[cfg(target_os = "windows")]
    {
        crate::platform::windows::install_or_update_snap_layout(
            &window,
            x,
            y,
            width,
            height,
            titlebar_height,
            control_band_width,
        )
    }
    #[cfg(not(target_os = "windows"))]
    {
        let _ = (
            window,
            x,
            y,
            width,
            height,
            titlebar_height,
            control_band_width,
        );
        Ok(())
    }
}
