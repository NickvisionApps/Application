mod views;

use crate::views::MainWindow;
use adw::{Application, ColorScheme, prelude::*};
use glib::ExitCode;
use shared::{APP_ID, AppState, ApplicationTheme};
use std::sync::Arc;

pub fn run() -> ExitCode {
    let state = Arc::new(AppState::default());
    let app: Application = Application::builder().application_id(APP_ID).build();
    let startup_state = state.clone();
    let activate_state = state.clone();
    app.connect_startup(move |app| {
        app.style_manager()
            .set_color_scheme(match startup_state.configuration().theme() {
                ApplicationTheme::Light => ColorScheme::ForceLight,
                ApplicationTheme::Dark => ColorScheme::ForceDark,
                _ => ColorScheme::Default,
            });
    });
    app.connect_activate(move |app| {
        let window = MainWindow::new(app, activate_state.clone());
        window.present();
    });
    app.run()
}
