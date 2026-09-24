use crate::product::ProductInfo;
use directories::BaseDirs;
use reup::UpdateType;
use serde::{Deserialize, Serialize};
use serde_repr::{Deserialize_repr, Serialize_repr};

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
    #[serde(skip)]
    app_name: String,
    allow_preview_updates: bool,
    #[serde(default = "default_true")]
    automatically_check_for_updates: bool,
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
    pub fn load(app_name: impl Into<String>) -> Result<Self, Box<dyn std::error::Error>> {
        let app_name = app_name.into();
        let path = if ProductInfo::is_portable() {
            std::env::current_exe()?
                .parent()
                .unwrap()
                .join("config.json")
        } else {
            BaseDirs::new()
                .ok_or("Unable to load base directories")?
                .config_dir()
                .join(&app_name)
                .join("config.json")
        };
        std::fs::create_dir_all(path.parent().unwrap())?;
        let mut config = if path.exists() {
            let json = std::fs::read_to_string(&path)?;
            serde_json::from_str(&json)?
        } else {
            Configuration::default()
        };
        config.set_app_name(app_name);
        Ok(config)
    }

    pub fn save(&self) -> Result<(), Box<dyn std::error::Error>> {
        let path = if ProductInfo::is_portable() {
            std::env::current_exe()?
                .parent()
                .unwrap()
                .join("config.json")
        } else {
            BaseDirs::new()
                .ok_or("Unable to load base directories")?
                .config_dir()
                .join(&self.app_name)
                .join("config.json")
        };
        std::fs::create_dir_all(path.parent().unwrap())?;
        let json = serde_json::to_string_pretty(self)?;
        std::fs::write(&path, json)?;
        Ok(())
    }

    pub fn set_app_name(&mut self, app_name: impl Into<String>) {
        self.app_name = app_name.into();
    }

    pub fn allow_preview_updates(&self) -> bool {
        self.allow_preview_updates
    }

    pub fn set_allow_preview_updates(&mut self, allow: bool) {
        self.allow_preview_updates = allow;
    }

    pub fn automatically_check_for_updates(&self) -> bool {
        self.automatically_check_for_updates
    }

    pub fn set_automatically_check_for_updates(&mut self, check: bool) {
        self.automatically_check_for_updates = check;
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

    pub fn update_type(&self) -> UpdateType {
        if self.allow_preview_updates {
            UpdateType::Preview
        } else {
            UpdateType::Stable
        }
    }
}

const fn default_true() -> bool {
    true
}
