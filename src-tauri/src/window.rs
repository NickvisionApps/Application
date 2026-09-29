use directories::BaseDirs;
use std::process::Command;

pub fn gnome_button_layout() -> Option<String> {
    Command::new("gsettings")
        .args(["get", "org.gnome.desktop.wm.preferences", "button-layout"])
        .output()
        .ok()
        .filter(|output| output.status.success())
        .and_then(|output| String::from_utf8(output.stdout).ok())
        .map(|value| value.trim().trim_matches('\'').to_string())
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
