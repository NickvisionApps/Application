use objc2::MainThreadMarker;
use objc2_app_kit::{NSColor, NSColorSpace};
use tauri::{AppHandle, WebviewWindow};
use window_vibrancy::{NSVisualEffectMaterial, NSVisualEffectState, apply_vibrancy};

pub fn apply_window_vibrancy(window: &WebviewWindow) {
    let _ = apply_vibrancy(
        window,
        NSVisualEffectMaterial::UnderWindowBackground,
        Some(NSVisualEffectState::FollowsWindowActiveState),
        None,
    );
}

pub fn accent_color(app: &AppHandle) -> Option<String> {
    let (tx, rx) = std::sync::mpsc::channel();
    app.run_on_main_thread(move || {
        let hex = MainThreadMarker::new().and_then(|_mtm| {
            let color = NSColor::controlAccentColor();
            let srgb = color.colorUsingColorSpace(&NSColorSpace::sRGBColorSpace())?;
            let r = (srgb.redComponent() * 255.0).round() as u8;
            let g = (srgb.greenComponent() * 255.0).round() as u8;
            let b = (srgb.blueComponent() * 255.0).round() as u8;
            Some(format!("#{r:02x}{g:02x}{b:02x}"))
        });
        let _ = tx.send(hex);
    })
    .ok()?;
    rx.recv().ok()?
}
