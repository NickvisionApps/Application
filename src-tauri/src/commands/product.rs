use crate::controller::AppController;
use crate::product::ProductInfo;
use std::sync::Mutex;
use tauri::{AppHandle, State, command};
use tauri_plugin_opener::OpenerExt;
use utente::User;

#[command]
pub fn get_debugging_information(controller: State<'_, Mutex<AppController>>) -> String {
    let controller = controller.lock().unwrap();
    let locale = std::env::var("LC_ALL")
        .or_else(|_| std::env::var("LC_MESSAGES"))
        .or_else(|_| std::env::var("LANG"))
        .ok()
        .or_else(sys_locale::get_locale)
        .unwrap_or_else(|| "N/A".to_string());
    let dir = std::env::current_exe()
        .or_else(|_| std::env::current_dir())
        .unwrap_or_default();
    #[cfg(target_os = "windows")]
    return format!(
        "```\nApp: {}\nVersion: {}\n\nOperating System: Windows\nDeployment Mode: {}\nLocale: {}\n\nIs Portable: {}\nRunning From: {}\n```",
        controller.product_info().name(),
        controller.product_info().version(),
        ProductInfo::deployment_mode(),
        locale,
        ProductInfo::is_portable(),
        dir.to_str().unwrap_or("N/A")
    );
    #[cfg(target_os = "macos")]
    return format!(
        "```\nApp: {}\nVersion: {}\n\nOperating System: macOS\nDeployment Mode: {}\nLocale: {}\n\nIs Portable: {}\nRunning From: {}\n```",
        controller.product_info().name(),
        controller.product_info().version(),
        ProductInfo::deployment_mode(),
        locale,
        ProductInfo::is_portable(),
        dir.to_str().unwrap_or("N/A")
    );
    #[cfg(target_os = "linux")]
    return format!(
        "```\nApp: {}\nVersion: {}\n\nOperating System: Linux\nDeployment Mode: {}\nLocale: {}\n\nIs Portable: {}\nRunning From: {}\n```",
        controller.product_info().name(),
        controller.product_info().version(),
        ProductInfo::deployment_mode(),
        locale,
        ProductInfo::is_portable(),
        dir.to_str().unwrap_or("N/A")
    );
}

#[command]
pub fn get_product_information(controller: State<'_, Mutex<AppController>>) -> ProductInfo {
    controller.lock().unwrap().product_info().clone()
}

#[command]
pub fn get_user() -> User {
    User::current()
}

#[command]
pub fn open_discussions(app: AppHandle, controller: State<'_, Mutex<AppController>>) -> bool {
    app.opener()
        .open_url(
            controller.lock().unwrap().product_info().discussions_url(),
            None::<&str>,
        )
        .is_ok()
}

#[command]
pub fn open_github_repository(app: AppHandle, controller: State<'_, Mutex<AppController>>) -> bool {
    app.opener()
        .open_url(
            controller.lock().unwrap().product_info().repo_url(),
            None::<&str>,
        )
        .is_ok()
}

#[command]
pub fn open_report_a_bug(app: AppHandle, controller: State<'_, Mutex<AppController>>) -> bool {
    app.opener()
        .open_url(
            controller.lock().unwrap().product_info().issues_url(),
            None::<&str>,
        )
        .is_ok()
}
