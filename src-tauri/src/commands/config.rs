use crate::config::Configuration;
use crate::controller::AppController;
use std::sync::Mutex;
use tauri::{State, command};

#[command]
pub fn get_configuration(controller: State<'_, Mutex<AppController>>) -> Configuration {
    let controller = controller.lock().unwrap();
    Configuration::new(
        controller.allow_preview_updates(),
        *controller.theme(),
        controller.translation_language().to_string(),
        *controller.window_geometry(),
    )
}

#[command]
pub fn set_configuration(
    configuration: Configuration,
    controller: State<'_, Mutex<AppController>>,
) -> Result<(), tauri::Error> {
    let mut controller = controller.lock().unwrap();
    controller.set_allow_preview_updates(configuration.allow_preview_updates());
    controller.set_theme(*configuration.theme());
    controller.set_translation_language(configuration.translation_language());
    controller.set_window_geometry(*configuration.window_geometry());
    controller
        .save()
        .map_err(|e| tauri::Error::Setup(e.into()))?;
    Ok(())
}
