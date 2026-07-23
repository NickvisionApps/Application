use crate::app_info;
use directories::BaseDirs;
use serde::{Deserialize, Serialize};

#[derive(Debug, Clone, Default, PartialEq, Eq, Serialize, Deserialize)]
pub enum ApplicationTheme {
    Light,
    Dark,
    #[default]
    System,
}

#[derive(Debug, Clone, PartialEq, Eq, Serialize, Deserialize)]
pub struct WindowGeometry {
    x: i32,
    y: i32,
    width: u32,
    height: u32,
    is_maximized: bool,
}

#[derive(Debug, Clone, Default, PartialEq, Eq)]
pub struct WindowGeometryBuilder {
    x: Option<i32>,
    y: Option<i32>,
    width: Option<u32>,
    height: Option<u32>,
    is_maximized: Option<bool>,
}

#[derive(Debug, Clone, Default, PartialEq, Eq, Serialize, Deserialize)]
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

    pub fn builder() -> WindowGeometryBuilder {
        WindowGeometryBuilder::default()
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

impl WindowGeometryBuilder {
    pub fn x(mut self, x: i32) -> Self {
        self.x = Some(x);
        self
    }

    pub fn y(mut self, y: i32) -> Self {
        self.y = Some(y);
        self
    }

    pub fn width(mut self, width: u32) -> Self {
        self.width = Some(width);
        self
    }

    pub fn height(mut self, height: u32) -> Self {
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
        let path = if app_info::is_portable() {
            std::env::current_exe()?
                .parent()
                .unwrap()
                .join("config.json")
        } else {
            BaseDirs::new()
                .expect("Unable to load base directories")
                .config_dir()
                .join(app_info::NAME)
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
            .join(app_info::NAME)
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

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn window_geometry_default() {
        let geometry = WindowGeometry::default();
        assert_eq!(geometry.x(), 100);
        assert_eq!(geometry.y(), 100);
        assert_eq!(geometry.width(), 800);
        assert_eq!(geometry.height(), 600);
        assert!(!geometry.is_maximized());
    }

    #[test]
    fn window_geometry_builder() {
        let geometry = WindowGeometry::builder()
            .x(200)
            .y(150)
            .width(1024)
            .height(768)
            .is_maximized(true)
            .build();
        assert_eq!(geometry.x(), 200);
        assert_eq!(geometry.y(), 150);
        assert_eq!(geometry.width(), 1024);
        assert_eq!(geometry.height(), 768);
        assert!(geometry.is_maximized());
    }

    #[test]
    fn configuration_default() {
        let config = Configuration::default();
        assert!(!config.allow_preview_updates());
        assert_eq!(config.theme(), &ApplicationTheme::System);
        assert_eq!(config.translation_language(), String::new());
        assert_eq!(config.window_geometry().x(), 100);
        assert_eq!(config.window_geometry().y(), 100);
        assert_eq!(config.window_geometry().width(), 800);
        assert_eq!(config.window_geometry().height(), 600);
        assert!(!config.window_geometry().is_maximized());
    }

    #[test]
    fn configuration_json() {
        let config = Configuration {
            allow_preview_updates: true,
            theme: ApplicationTheme::Dark,
            translation_language: "en".to_string(),
            window_geometry: WindowGeometry::new(100, 100, 800, 600, false),
        };
        let json = serde_json::to_string(&config).unwrap();
        let deserialized: Configuration = serde_json::from_str(&json).unwrap();
        assert_eq!(config, deserialized);
    }

    #[test]
    fn configuration_save_load() {
        let mut config = Configuration::default();
        config.set_allow_preview_updates(true);
        config.set_theme(ApplicationTheme::Dark);
        config.set_translation_language("en".to_string());
        config.set_window_geometry(WindowGeometry::new(200, 150, 1024, 768, true));
        config.save().unwrap();
        let loaded_config = Configuration::load().unwrap();
        assert_eq!(config, loaded_config);
        std::fs::remove_file(
            BaseDirs::new()
                .expect("Unable to load base directories")
                .config_dir()
                .join(app_info::NAME)
                .join("config.json"),
        )
        .unwrap();
    }
}
