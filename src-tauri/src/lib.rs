pub mod close;
pub mod commands;
pub mod config;
pub mod folder;
pub mod product;
pub mod translation;
pub mod update;

use crate::close::CloseManager;
use crate::config::Configuration;
use crate::product::ProductInfo;
use crate::translation::Translator;
use semver::Version;
use std::sync::Mutex;
use tauri::Manager;

#[cfg_attr(mobile, tauri::mobile_entry_point)]
pub fn run() {
    tauri::Builder::default()
        .plugin(tauri_plugin_dialog::init())
        .plugin(tauri_plugin_opener::init())
        .plugin(tauri_plugin_os::init())
        .plugin(tauri_plugin_process::init())
        .plugin(tauri_plugin_updater::Builder::default().build())
        .plugin(tauri_plugin_window_state::Builder::default().build())
        .setup(|app| {
            let product_info = ProductInfo::builder()
                .id("org.nickvision.application")
                .name("Nickvision Application")
                .short_name("Application")
                .repo_owner("NickvisionApps")
                .repo_name("Application")
                .version(Version::parse(&format!("{}-next", env!("CARGO_PKG_VERSION"))).unwrap())
                .build()
                .unwrap();
            let mut configuration = Configuration::load(product_info.name()).unwrap_or_default();
            let translator = Translator::new(
                product_info.short_name(),
                app.path().resource_dir()?.join("locale"),
                Some(configuration.translation_language()),
            );
            configuration.set_translation_language(translator.language());
            app.manage(product_info);
            app.manage(Mutex::new(configuration));
            app.manage(Mutex::new(translator));
            app.manage(Mutex::new(CloseManager::default()));
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
            commands::update::get_new_update,
            commands::update::install_update,
            commands::window::can_window_close,
            commands::window::confirm_window_close
        ])
        .run(tauri::generate_context!())
        .expect("Error while running tauri application");
}
