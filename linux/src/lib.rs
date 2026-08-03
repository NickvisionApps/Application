mod views;

use crate::views::MainWindow;
use adw::{Application, ColorScheme, prelude::*};
use glib::ExitCode;
use shared::{APP_ID, AppState, ApplicationTheme};
use std::{cell::RefCell, rc::Rc};

pub fn run() -> ExitCode {
    let state = Rc::new(RefCell::new(AppState::default()));
    gio::resources_register_include!("compiled.gresource").unwrap();
    let app: Application = Application::builder().application_id(APP_ID).build();
    let startup_state = state.clone();
    let activate_state = state.clone();
    app.connect_startup(move |app| {
        let state_ref = startup_state.borrow();
        app.style_manager()
            .set_color_scheme(match state_ref.configuration().theme() {
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
