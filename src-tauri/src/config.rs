use crate::info;
use directories::BaseDirs;
use serde::{Deserialize, Serialize};
use serde_repr::{Deserialize_repr, Serialize_repr};
use std::path::PathBuf;

#[derive(Debug, Clone, Copy, Default, PartialEq, Eq, Serialize_repr, Deserialize_repr)]
#[repr(u8)]
pub enum ApplicationTheme {
    Light,
    Dark,
    #[default]
    System,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, Serialize, Deserialize)]
#[serde(rename_all = "camelCase")]
pub struct WindowGeometry {
    x: i32,
    y: i32,
    width: u32,
    height: u32,
    is_maximized: bool,
}

#[derive(Debug, Clone, Default, PartialEq, Eq, Serialize, Deserialize)]
#[serde(rename_all = "camelCase")]
pub struct Configuration {
    allow_preview_updates: bool,
    theme: ApplicationTheme,
    translation_language: String,
    window_geometry: WindowGeometry,
}

impl WindowGeometry {
    pub fn new(x: i32, y: i32, width: u32, height: u32, is_maximized: bool) -> Self {
        WindowGeometry {
            x,
            y,
            width,
            height,
            is_maximized,
        }
    }

    pub fn x(&self) -> i32 {
        self.x
    }

    pub fn y(&self) -> i32 {
        self.y
    }

    pub fn width(&self) -> u32 {
        self.width
    }

    pub fn height(&self) -> u32 {
        self.height
    }

    pub fn is_maximized(&self) -> bool {
        self.is_maximized
    }
}

impl Default for WindowGeometry {
    fn default() -> Self {
        Self {
            x: 100,
            y: 100,
            width: 800,
            height: 600,
            is_maximized: false,
        }
    }
}

impl Configuration {
    pub fn new(
        allow_preview_updates: bool,
        theme: ApplicationTheme,
        translation_language: String,
        window_geometry: WindowGeometry,
    ) -> Self {
        Self {
            allow_preview_updates,
            theme,
            translation_language,
            window_geometry,
        }
    }

    pub fn load() -> Result<Self, Box<dyn std::error::Error>> {
        let path = Self::config_path()?;
        std::fs::create_dir_all(path.parent().unwrap())?;
        if path.exists() {
            let json = std::fs::read_to_string(&path)?;
            let config: Configuration = serde_json::from_str(&json)?;
            Ok(config)
        } else {
            Ok(Configuration::default())
        }
    }

    pub fn save(&self) -> Result<(), Box<dyn std::error::Error>> {
        let path = Self::config_path()?;
        std::fs::create_dir_all(path.parent().unwrap())?;
        let json = serde_json::to_string_pretty(self)?;
        std::fs::write(&path, json)?;
        Ok(())
    }

    pub fn allow_preview_updates(&self) -> bool {
        self.allow_preview_updates
    }

    pub fn set_allow_preview_updates(&mut self, allow: bool) {
        self.allow_preview_updates = allow;
    }

    pub fn theme(&self) -> &ApplicationTheme {
        &self.theme
    }

    pub fn set_theme(&mut self, theme: ApplicationTheme) {
        self.theme = theme;
    }

    pub fn translation_language(&self) -> &str {
        &self.translation_language
    }

    pub fn set_translation_language(&mut self, language: impl Into<String>) {
        self.translation_language = language.into();
    }

    pub fn window_geometry(&self) -> &WindowGeometry {
        &self.window_geometry
    }

    pub fn set_window_geometry(&mut self, geometry: WindowGeometry) {
        self.window_geometry = geometry;
    }

    fn config_path() -> Result<PathBuf, Box<dyn std::error::Error>> {
        Ok(if info::is_app_portable() {
            std::env::current_exe()?
                .parent()
                .unwrap()
                .join("config.json")
        } else {
            BaseDirs::new()
                .ok_or("Unable to load base directories")?
                .config_dir()
                .join(info::APP_NAME)
                .join("config.json")
        })
    }
}
