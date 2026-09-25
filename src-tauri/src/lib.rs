pub mod close;
pub mod commands;
pub mod config;
pub mod folder;
pub mod product;
pub mod translation;

use crate::close::CloseManager;
use crate::config::Configuration;
use crate::product::ProductInfo;
use crate::translation::Translator;
use reup::GitHubUpdater;
use semver::Version;
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
            #[cfg(all(target_os = "windows", target_arch = "x86_64"))]
            let updater = GitHubUpdater::new(
                product_info.repo_owner(),
                product_info.repo_name(),
                "NickvisionApplicationSetup.exe",
            );
            #[cfg(all(target_os = "windows", target_arch = "aarch64"))]
            let updater = GitHubUpdater::new(
                product_info.repo_owner(),
                product_info.repo_name(),
                "NickvisionApplicationSetup-arm64.exe",
            );
            #[cfg(all(target_os = "linux", target_arch = "x86_64"))]
            let updater = GitHubUpdater::new(
                product_info.repo_owner(),
                product_info.repo_name(),
                "org.nickvision.application.x64.flatpak",
            );
            #[cfg(all(target_os = "linux", target_arch = "aarch64"))]
            let updater = GitHubUpdater::new(
                product_info.repo_owner(),
                product_info.repo_name(),
                "org.nickvision.application.aarch64.flatpak",
            );
            #[cfg(all(target_os = "macos", target_arch = "x86_64"))]
            let updater = GitHubUpdater::new(
                product_info.repo_owner(),
                product_info.repo_name(),
                "Application-macOS-x64.zip",
            );
            #[cfg(all(target_os = "macos", target_arch = "aarch64"))]
            let updater = GitHubUpdater::new(
                product_info.repo_owner(),
                product_info.repo_name(),
                "Application-macOS-arm64.zip",
            );
            configuration.set_translation_language(translator.language());
            app.manage(product_info);
            app.manage(Mutex::new(configuration));
            app.manage(Mutex::new(translator));
            app.manage(updater);
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
            commands::updater::check_for_updates,
            commands::updater::install_update,
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
                .state::<Mutex<CloseManager>>()
                .lock()
                .unwrap()
                .set_can_close(false);
            window.show().unwrap();
            window.set_focus().unwrap();
        }
    });
}
