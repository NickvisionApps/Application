use crate::product::ProductInfo;
use tauri::{AppHandle, State, command};
use tauri_plugin_opener::OpenerExt;
use utente::User;

#[command]
pub fn get_debugging_information(product_info: State<'_, ProductInfo>) -> String {
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
        product_info.name(),
        product_info.version(),
        ProductInfo::deployment_mode(),
        locale,
        ProductInfo::is_portable(),
        dir.to_str().unwrap_or("N/A")
    );
    #[cfg(target_os = "macos")]
    return format!(
        "```\nApp: {}\nVersion: {}\n\nOperating System: macOS\nDeployment Mode: {}\nLocale: {}\n\nIs Portable: {}\nRunning From: {}\n```",
        product_info.name(),
        product_info.version(),
        ProductInfo::deployment_mode(),
        locale,
        ProductInfo::is_portable(),
        dir.to_str().unwrap_or("N/A")
    );
    #[cfg(target_os = "linux")]
    return format!(
        "```\nApp: {}\nVersion: {}\n\nOperating System: Linux\nDeployment Mode: {}\nLocale: {}\n\nIs Portable: {}\nRunning From: {}\n```",
        product_info.name(),
        product_info.version(),
        ProductInfo::deployment_mode(),
        locale,
        ProductInfo::is_portable(),
        dir.to_str().unwrap_or("N/A")
    );
}

#[command]
pub fn get_product_information(product_info: State<'_, ProductInfo>) -> ProductInfo {
    product_info.inner().clone()
}

#[command]
pub fn get_user() -> User {
    User::current()
}

#[command]
pub fn open_discussions(app: AppHandle, product_info: State<'_, ProductInfo>) -> bool {
    app.opener()
        .open_url(product_info.discussions_url(), None::<&str>)
        .is_ok()
}

#[command]
pub fn open_github_repository(app: AppHandle, product_info: State<'_, ProductInfo>) -> bool {
    app.opener()
        .open_url(product_info.repo_url(), None::<&str>)
        .is_ok()
}

#[command]
pub fn open_report_a_bug(app: AppHandle, product_info: State<'_, ProductInfo>) -> bool {
    app.opener()
        .open_url(product_info.issues_url(), None::<&str>)
        .is_ok()
}
