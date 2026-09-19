use crate::config::WindowGeometry;
use crate::controller::AppController;
use crate::translation::Translator;
use std::sync::Mutex;
use tauri::{Manager, State, WebviewWindow, Window, WindowEvent, command};
use tauri_plugin_decoration::WebviewWindowExt;

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
                        && let Ok(size) = window.inner_size()
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
