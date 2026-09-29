use directories::BaseDirs;

const TITLEBAR_ICON_CANDIDATES: [(&str, &[&str]); 4] = [
    ("minimize", &["window-minimize-symbolic", "window-minimize"]),
    ("maximize", &["window-maximize-symbolic", "window-maximize"]),
    ("restore", &["window-restore-symbolic", "window-restore"]),
    ("close", &["window-close-symbolic", "window-close"]),
];

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
        let provider = gtk::CssProvider::new();
        provider
            .load_from_data(
                b"window.csd decoration {\n\
                    border-radius: 12px;\n\
                  }\n\
                  window.csd.maximized decoration,\n\
                  window.csd.fullscreen decoration,\n\
                  window.csd.tiled decoration {\n\
                    border-radius: 0;\n\
                  }",
            )
            .unwrap();
        if let Some(screen) = gtk_window.screen() {
            gtk::StyleContext::add_provider_for_screen(
                &screen,
                &provider,
                gtk::STYLE_PROVIDER_PRIORITY_APPLICATION,
            );
        }
    } else {
        gtk_window.set_decorated(false);
    }
}

pub fn button_layout() -> Option<String> {
    use gtk::prelude::*;
    gtk::Settings::default()?
        .gtk_decoration_layout()
        .map(|value| value.to_string())
        .or_else(|| {
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
