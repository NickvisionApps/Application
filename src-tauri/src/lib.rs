pub mod commands;
pub mod config;
pub mod controller;
pub mod folder;
pub mod info;
pub mod translation;

use crate::controller::AppController;
use std::sync::Mutex;
use tauri::Manager;

#[cfg_attr(mobile, tauri::mobile_entry_point)]
pub fn run() {
    let app = tauri::Builder::default()
        .plugin(tauri_plugin_decoration::init())
        .plugin(tauri_plugin_dialog::init())
        .plugin(tauri_plugin_opener::init())
        .plugin(tauri_plugin_os::init())
        .manage(Mutex::new(AppController::default()))
        .setup(|app| {
            translation::init(app.path().resource_dir()?.join("locale"));
            translation::set_language(
                app.state::<Mutex<AppController>>()
                    .lock()
                    .unwrap()
                    .translation_language()
                    .to_string(),
            );
            Ok(())
        })
        .invoke_handler(tauri::generate_handler![
            commands::config::get_configuration,
            commands::config::set_configuration,
            commands::folder::close_folder,
            commands::folder::open_folder,
            commands::info::get_debugging_information,
            commands::info::get_version,
            commands::info::get_user,
            commands::info::open_discussions,
            commands::info::open_github_repository,
            commands::info::open_report_a_bug,
            commands::translation::get_available_translation_languages,
            commands::translation::translate_f,
            commands::translation::translate_g,
            commands::translation::translate_n,
            commands::translation::translate_nf,
            commands::translation::translate_np,
            commands::translation::translate_npf,
            commands::translation::translate_p,
            commands::translation::translate_pf,
            commands::window::show_main_window,
        ])
        .on_window_event(|window, event| {
            commands::window::handle_window_event(window, event);
        })
        .build(tauri::generate_context!())
        .expect("Error while running tauri application");
    app.run(|handle, event| {
        #[cfg(target_os = "macos")]
        if let tauri::RunEvent::Reopen {
            has_visible_windows,
            ..
        } = event
            && !has_visible_windows
            && let Some(window) = handle.get_webview_window("main")
        {
            window.show().unwrap();
            window.set_focus().unwrap();
        }
    });
}
