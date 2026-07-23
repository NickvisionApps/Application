#[cfg(not(any(target_os = "windows", target_os = "macos", target_os = "linux")))]
compile_error!("armoire only supports Windows, macOS, and Linux");

pub mod app_info;
mod config;
mod folder;
mod translation;

pub use config::{ApplicationTheme, Configuration, WindowGeometry, WindowGeometryBuilder};
pub use folder::FolderBrowser;
pub use translation::Translator;

pub struct AppState {
    configuration: Configuration,
    folder_browser: FolderBrowser,
    translator: Translator,
}

impl AppState {
    pub fn new() -> Self {
        let configuration = Configuration::load().unwrap();
        let folder_browser = FolderBrowser::default();
        let translator = Translator::new(configuration.translation_language());
        AppState {
            configuration,
            folder_browser,
            translator,
        }
    }

    pub fn configuration(&mut self) -> &mut Configuration {
        &mut self.configuration
    }

    pub fn folder_browser(&mut self) -> &mut FolderBrowser {
        &mut self.folder_browser
    }

    pub fn translator(&self) -> &Translator {
        &self.translator
    }
}

impl Default for AppState {
    fn default() -> Self {
        AppState::new()
    }
}
