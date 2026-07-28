use crate::{Configuration, FolderBrowser, Translator, Updater};
use std::sync::Arc;

#[derive(Debug, Clone)]
pub struct AppState {
    configuration: Configuration,
    folder_browser: FolderBrowser,
    translator: Arc<Translator>,
    updater: Arc<Updater>,
}

impl AppState {
    pub fn new() -> Self {
        AppState::default()
    }

    pub fn can_close(&self) -> bool {
        true
    }

    pub fn configuration(&self) -> &Configuration {
        &self.configuration
    }

    pub fn configuration_mut(&mut self) -> &mut Configuration {
        &mut self.configuration
    }

    pub fn folder_browser(&self) -> &FolderBrowser {
        &self.folder_browser
    }

    pub fn folder_browser_mut(&mut self) -> &mut FolderBrowser {
        &mut self.folder_browser
    }

    pub fn translator(&self) -> Arc<Translator> {
        Arc::clone(&self.translator)
    }

    pub fn updater(&self) -> Arc<Updater> {
        Arc::clone(&self.updater)
    }
}

impl Default for AppState {
    fn default() -> Self {
        let configuration = Configuration::load().unwrap();
        let folder_browser = FolderBrowser::default();
        let translator = Arc::new(Translator::new(configuration.translation_language()));
        let updater = Arc::new(Updater::new(configuration.allow_preview_updates()));
        AppState {
            configuration,
            folder_browser,
            translator,
            updater,
        }
    }
}
