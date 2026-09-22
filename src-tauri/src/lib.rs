pub mod commands;
pub mod config;
pub mod controller;
pub mod folder;
pub mod product;
pub mod translation;

use crate::controller::AppController;
use crate::translation::Translator;
use std::sync::Mutex;
use std::time::Duration;
use tauri::Manager;

#[cfg_attr(mobile, tauri::mobile_entry_point)]
pub fn run() {
    let app = tauri::Builder::default()
        .plugin(
            tauri_plugin_decoration::builder()
                .activation_timeout(Duration::from_secs(15))
                .build(),
        )
        .plugin(tauri_plugin_dialog::init())
        .plugin(tauri_plugin_opener::init())
        .plugin(tauri_plugin_os::init())
        .manage(Mutex::new(AppController::default()))
        .setup(|app| {
            let mutex = app.state::<Mutex<AppController>>();
            let mut controller = mutex.lock().unwrap();
            let translator = Translator::new(
                controller.product_info().short_name(),
                app.path().resource_dir()?.join("locale"),
                Some(controller.translation_language()),
            );
            controller.set_translation_language(translator.language());
            app.manage(Mutex::new(translator));
            Ok(())
        })
        .invoke_handler(tauri::generate_handler![
            commands::config::get_configuration,
            commands::config::set_configuration,
            commands::folder::close_folder,
            commands::folder::open_folder,
            commands::product::get_debugging_information,
            commands::product::get_product_information,
            commands::product::get_user,
            commands::product::open_discussions,
            commands::product::open_github_repository,
            commands::product::open_report_a_bug,
            commands::translation::get_available_translation_languages,
            commands::translation::translate_f,
            commands::translation::translate_g,
            commands::translation::translate_n,
            commands::translation::translate_nf,
            commands::translation::translate_np,
            commands::translation::translate_npf,
            commands::translation::translate_p,
            commands::translation::translate_pf,
            commands::window::can_window_close,
            commands::window::confirm_window_close,
            commands::window::show_main_window,
        ])
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
            handle
                .state::<Mutex<AppController>>()
                .lock()
                .unwrap()
                .reset_close();
            window.show().unwrap();
            window.set_focus().unwrap();
        }
    });
}
