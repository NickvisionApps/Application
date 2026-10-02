use directories::BaseDirs;

const TITLEBAR_ICON_CANDIDATES: [(&str, &[&str]); 4] = [
    ("minimize", &["window-minimize-symbolic", "window-minimize"]),
    ("maximize", &["window-maximize-symbolic", "window-maximize"]),
    ("restore", &["window-restore-symbolic", "window-restore"]),
    ("close", &["window-close-symbolic", "window-close"]),
];

pub fn button_layout() -> Option<String> {
    use gtk::prelude::*;
    gtk::Settings::default()?
        .gtk_decoration_layout()
        .map(|value| value.to_string())
        .or_else(|| {
            let contents =
                std::fs::read_to_string(BaseDirs::new()?.config_dir().join("kwinrc")).ok()?;
            let mut in_section = false;
            let mut left = String::default();
            let mut right = String::default();
            for line in contents.lines() {
                let line = line.trim();
                if line.starts_with('[') {
                    in_section = line == "[org.kde.kdecoration2]";
                    continue;
                }
                if !in_section {
                    continue;
                }
                if let Some(value) = line.strip_prefix("ButtonsOnLeft=") {
                    left = value.to_string();
                } else if let Some(value) = line.strip_prefix("ButtonsOnRight=") {
                    right = value.to_string();
                }
            }
            Some(format!(
                "{}:{}",
                translate_kde_codes(&left),
                translate_kde_codes(&right)
            ))
        })
}

pub fn titlebar_icons(
    window: &tauri::WebviewWindow,
) -> Option<std::collections::BTreeMap<&'static str, String>> {
    use base64::{Engine as _, engine::general_purpose::STANDARD};
    use gtk::prelude::*;
    let gtk_window = window.gtk_window().ok()?;
    let theme = gtk::IconTheme::for_screen(&GtkWindowExt::screen(&gtk_window)?)?;
    let style_context = gtk_window.style_context();
    let scale = gtk_window.scale_factor().clamp(1, 4);
    let mut icons = std::collections::BTreeMap::new();
    for (control, candidates) in TITLEBAR_ICON_CANDIDATES {
        for candidate in candidates {
            let Some(icon) =
                theme.lookup_icon_for_scale(candidate, 16, scale, gtk::IconLookupFlags::FORCE_SIZE)
            else {
                continue;
            };
            let Ok((pixbuf, _)) = icon.load_symbolic_for_context(&style_context) else {
                continue;
            };
            let Ok(png) = pixbuf.save_to_bufferv("png", &[]) else {
                continue;
            };
            icons.insert(
                control,
                format!("data:image/png;base64,{}", STANDARD.encode(png)),
            );
            break;
        }
    }
    (!icons.is_empty()).then_some(icons)
}

const GNOME_ACCENT_COLORS: [(&str, &str); 9] = [
    ("blue", "#3584e4"),
    ("teal", "#2190a4"),
    ("green", "#3a944a"),
    ("yellow", "#c88800"),
    ("orange", "#ed5b00"),
    ("red", "#e62d42"),
    ("pink", "#d56199"),
    ("purple", "#9141ac"),
    ("slate", "#6f8396"),
];

pub fn accent_color() -> Option<String> {
    std::process::Command::new("gsettings")
        .args(["get", "org.gnome.desktop.interface", "accent-color"])
        .output()
        .ok()
        .filter(|output| output.status.success())
        .and_then(|output| {
            let name = String::from_utf8_lossy(&output.stdout);
            let name = name.trim().trim_matches('\'');
            GNOME_ACCENT_COLORS
                .iter()
                .find(|(candidate, _)| *candidate == name)
                .map(|(_, hex)| hex.to_string())
        })
        .or_else(|| {
            let contents =
                std::fs::read_to_string(BaseDirs::new()?.config_dir().join("kdeglobals")).ok()?;
            let mut in_section = false;
            for line in contents.lines() {
                let line = line.trim();
                if line.starts_with('[') {
                    in_section = line == "[General]";
                    continue;
                }
                if !in_section {
                    continue;
                }
                if let Some(value) = line.strip_prefix("AccentColor=") {
                    let parts: Vec<&str> = value.split(',').collect();
                    if let [r, g, b] = parts[..] {
                        let (r, g, b) = (r.parse::<u8>().ok()?, g.parse::<u8>().ok()?, b.parse::<u8>().ok()?);
                        return Some(format!("#{r:02x}{g:02x}{b:02x}"));
                    }
                }
            }
            None
        })
}

fn translate_kde_codes(codes: &str) -> String {
    codes
        .chars()
        .filter_map(|code| match code {
            'I' => Some("minimize"),
            'A' => Some("maximize"),
            'X' => Some("close"),
            _ => None,
        })
        .collect::<Vec<_>>()
        .join(",")
}
