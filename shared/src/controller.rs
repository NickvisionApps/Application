use crate::{Configuration, FolderBrowser, Translator, Updater};
use chrono::{Local, Timelike};
use std::ops::{Deref, DerefMut};

#[derive(Debug)]
pub struct AppController {
    configuration: Configuration,
    folder_browser: FolderBrowser,
    translator: Translator,
    updater: Updater,
}

impl AppController {
    pub fn new() -> Self {
        AppController::default()
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

    pub fn greeting(&self) -> String {
        let hours = Local::now().hour();
        if hours < 6 {
            self.translator._p("Night", "Good Morning!")
        } else if hours < 12 {
            self.translator._p("Morning", "Good Morning!")
        } else if hours < 18 {
            self.translator._g("Good Afternoon!")
        } else {
            self.translator._g("Good Evening!")
        }
    }

    pub fn translator(&self) -> &Translator {
        &self.translator
    }

    pub fn updater(&self) -> &Updater {
        &self.updater
    }
}

impl Default for AppController {
    fn default() -> Self {
        let configuration = Configuration::load().unwrap_or_else(|_| Configuration::default());
        let folder_browser = FolderBrowser::default();
        let translator = Translator::new(configuration.translation_language());
        let updater = Updater::new(configuration.allow_preview_updates());
        AppController {
            configuration,
            folder_browser,
            translator,
            updater,
        }
    }
}

impl Deref for AppController {
    type Target = Configuration;

    fn deref(&self) -> &Self::Target {
        &self.configuration
    }
}

impl DerefMut for AppController {
    fn deref_mut(&mut self) -> &mut Self::Target {
        &mut self.configuration
    }
}

impl AsRef<Configuration> for AppController {
    fn as_ref(&self) -> &Configuration {
        &self.configuration
    }
}

impl AsRef<FolderBrowser> for AppController {
    fn as_ref(&self) -> &FolderBrowser {
        &self.folder_browser
    }
}

impl AsRef<Translator> for AppController {
    fn as_ref(&self) -> &Translator {
        &self.translator
    }
}

impl AsRef<Updater> for AppController {
    fn as_ref(&self) -> &Updater {
        &self.updater
    }
}
