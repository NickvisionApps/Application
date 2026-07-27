#[cfg(not(any(target_os = "windows", target_os = "macos", target_os = "linux")))]
compile_error!("armoire only supports Windows, macOS, and Linux");

mod config;
mod folder;
pub mod info;
mod state;
mod translation;

pub use config::{ApplicationTheme, Configuration, WindowGeometry, WindowGeometryBuilder};
pub use folder::FolderBrowser;
pub use state::AppState;
pub use translation::Translator;
