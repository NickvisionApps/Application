use crate::config::Configuration;
use crate::translation::Translator;
use std::sync::Mutex;
use tauri::{State, command};

#[command]
pub fn get_configuration(configuration: State<'_, Mutex<Configuration>>) -> Configuration {
    configuration.lock().unwrap().clone()
}

#[command]
pub fn set_configuration(
    new_configuration: Configuration,
    configuration: State<'_, Mutex<Configuration>>,
    translator: State<'_, Mutex<Translator>>,
) -> Result<(), tauri::Error> {
    let mut configuration = configuration.lock().unwrap();
    configuration.set_allow_preview_updates(new_configuration.allow_preview_updates());
    configuration
        .set_automatically_check_for_updates(new_configuration.automatically_check_for_updates());
    configuration.set_theme(*new_configuration.theme());
    configuration.set_translation_language(new_configuration.translation_language());
    configuration.set_window_geometry(*new_configuration.window_geometry());
    configuration
        .save()
        .map_err(|e| tauri::Error::Setup(e.into()))?;
    translator
        .lock()
        .unwrap()
        .set_language(configuration.translation_language());
    Ok(())
}
