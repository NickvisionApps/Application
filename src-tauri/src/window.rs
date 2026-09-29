use directories::BaseDirs;

#[cfg(target_os = "linux")]
pub fn gnome_button_layout() -> Option<String> {
    gtk::Settings::default()?.gtk_decoration_layout()
}

#[cfg(target_os = "linux")]
pub fn apply_native_decorations(window: &tauri::WebviewWindow) {
    use gtk::prelude::*;
    let Ok(gtk_window) = window.gtk_window() else {
        return;
    };
    if gtk_window.display().type_().name() == "GdkWaylandDisplay" {
        let titlebar = gtk::Box::new(gtk::Orientation::Horizontal, 0);
        titlebar.set_size_request(0, 0);
        titlebar.show();
        gtk_window.set_titlebar(Some(&titlebar));
    } else {
        gtk_window.set_decorated(false);
    }
}

pub fn kde_button_layout() -> Result<String, Box<dyn std::error::Error>> {
    let contents = std::fs::read_to_string(
        BaseDirs::new()
            .ok_or("Unable to load base directories")?
            .config_dir()
            .join("kwinrc"),
    )?;
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
    Ok(format!(
        "{}:{}",
        translate_kde_codes(&left),
        translate_kde_codes(&right)
    ))
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
