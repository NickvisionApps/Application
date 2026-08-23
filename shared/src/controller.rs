use crate::{_g, _p, Configuration, FolderBrowser, Updater, set_translation_language};
use chrono::{Local, Timelike};
use std::ops::{Deref, DerefMut};

#[derive(Debug)]
pub struct AppController {
    configuration: Configuration,
    folder_browser: FolderBrowser,
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
            _p("Night", "Good Morning!")
        } else if hours < 12 {
            _p("Morning", "Good Morning!")
        } else if hours < 18 {
            _g("Good Afternoon!")
        } else {
            _g("Good Evening!")
        }
    }

    pub fn updater(&self) -> &Updater {
        &self.updater
    }
}

impl Default for AppController {
    fn default() -> Self {
        let configuration = Configuration::load().unwrap_or_else(|_| Configuration::default());
        let folder_browser = FolderBrowser::default();
        let updater = Updater::new(configuration.allow_preview_updates());
        set_translation_language(configuration.translation_language());
        AppController {
            configuration,
            folder_browser,
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

impl AsRef<Updater> for AppController {
    fn as_ref(&self) -> &Updater {
        &self.updater
    }
}
