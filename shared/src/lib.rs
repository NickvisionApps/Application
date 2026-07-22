#[cfg(not(any(target_os = "windows", target_os = "macos", target_os = "linux")))]
compile_error!("armoire only supports Windows, macOS, and Linux");

pub mod config;
mod info;
mod translation;

pub use info::AppInfo;
pub use translation::Translator;
