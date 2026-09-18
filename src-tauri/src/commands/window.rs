use crate::config::WindowGeometry;
use crate::controller::AppController;
use crate::translation::_g;
use std::sync::Mutex;
use tauri::{Manager, Window, WindowEvent, command};

pub fn handle_window_event(window: &Window, event: &WindowEvent) {
    if let WindowEvent::CloseRequested { api, .. } = event {
        let state = window.state::<Mutex<AppController>>();
        let mut controller = state.lock().unwrap();
        if !controller.can_close() {
            api.prevent_close();
        } else {
            controller.set_window_geometry(
                if let Ok(maximized) = window.is_maximized()
                    && maximized
                {
                    WindowGeometry::new(10, 10, 800, 600, true)
                } else {
                    if let Ok(scale) = window.scale_factor()
                        && let Ok(size) = window.outer_size()
                        && let Ok(position) = window.outer_position()
                    {
                        let logical_size = size.to_logical(scale);
                        let logical_position = position.to_logical(scale);
                        WindowGeometry::new(
                            logical_position.x,
                            logical_position.y,
                            logical_size.width,
                            logical_size.height,
                            false,
                        )
                    } else {
                        WindowGeometry::default()
                    }
                },
            );
            controller.save().unwrap();
        }
        #[cfg(target_os = "macos")]
        {
            window.hide().unwrap();
            api.prevent_close();
        }
    }
}

#[command]
pub fn show_main_window(window: Window) -> Result<(), tauri::Error> {
    let window = window
        .get_webview_window("main")
        .ok_or(tauri::Error::AssetNotFound(
            "Main window not found".to_string(),
        ))?;
    window.set_title(&_g("Application"))?;
    window.show()?;
    window.set_focus()?;
    Ok(())
}
