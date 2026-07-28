use crate::{APP_NAME, is_app_portable};
use directories::BaseDirs;
use serde::{Deserialize, Serialize};
use serde_repr::{Deserialize_repr, Serialize_repr};

#[derive(Debug, Clone, Default, PartialEq, Eq, Serialize_repr, Deserialize_repr)]
#[repr(u8)]
pub enum ApplicationTheme {
    Light,
    Dark,
    #[default]
    System,
}

#[derive(Debug, Clone, PartialEq, Eq, Serialize, Deserialize)]
#[serde(rename_all = "PascalCase")]
pub struct WindowGeometry {
    x: i64,
    y: i64,
    width: u64,
    height: u64,
    is_maximized: bool,
}

#[derive(Debug, Clone, Default, PartialEq, Eq)]
pub struct WindowGeometryBuilder {
    x: Option<i64>,
    y: Option<i64>,
    width: Option<u64>,
    height: Option<u64>,
    is_maximized: Option<bool>,
}

#[derive(Debug, Clone, Default, PartialEq, Eq, Serialize, Deserialize)]
#[serde(rename_all = "PascalCase")]
pub struct Configuration {
    allow_preview_updates: bool,
    theme: ApplicationTheme,
    translation_language: String,
    window_geometry: WindowGeometry,
}

impl WindowGeometry {
    pub fn new(x: i64, y: i64, width: u64, height: u64, is_maximized: bool) -> Self {
        WindowGeometry {
            x,
            y,
            width,
            height,
            is_maximized,
        }
    }

    pub fn builder() -> WindowGeometryBuilder {
        WindowGeometryBuilder::default()
    }

    pub fn x(&self) -> i64 {
        self.x
    }

    pub fn y(&self) -> i64 {
        self.y
    }

    pub fn width(&self) -> u64 {
        self.width
    }

    pub fn height(&self) -> u64 {
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

impl WindowGeometryBuilder {
    pub fn new() -> Self {
        WindowGeometryBuilder::default()
    }

    pub fn x(mut self, x: i64) -> Self {
        self.x = Some(x);
        self
    }

    pub fn y(mut self, y: i64) -> Self {
        self.y = Some(y);
        self
    }

    pub fn width(mut self, width: u64) -> Self {
        self.width = Some(width);
        self
    }

    pub fn height(mut self, height: u64) -> Self {
        self.height = Some(height);
        self
    }

    pub fn is_maximized(mut self, is_maximized: bool) -> Self {
        self.is_maximized = Some(is_maximized);
        self
    }

    pub fn build(self) -> WindowGeometry {
        WindowGeometry {
            x: self.x.unwrap_or(100),
            y: self.y.unwrap_or(100),
            width: self.width.unwrap_or(800),
            height: self.height.unwrap_or(600),
            is_maximized: self.is_maximized.unwrap_or(false),
        }
    }
}

impl Configuration {
    pub fn load() -> Result<Self, Box<dyn std::error::Error>> {
        let path = if is_app_portable() {
            std::env::current_exe()?
                .parent()
                .unwrap()
                .join("config.json")
        } else {
            BaseDirs::new()
                .expect("Unable to load base directories")
                .config_dir()
                .join(APP_NAME)
                .join("config.json")
        };
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
        let path = BaseDirs::new()
            .expect("Unable to load base directories")
            .config_dir()
            .join(APP_NAME)
            .join("config.json");
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

    pub fn set_translation_language(&mut self, language: String) {
        self.translation_language = language;
    }

    pub fn window_geometry(&self) -> &WindowGeometry {
        &self.window_geometry
    }

    pub fn set_window_geometry(&mut self, geometry: WindowGeometry) {
        self.window_geometry = geometry;
    }
}
