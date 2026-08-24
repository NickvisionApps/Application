mod controls;
mod views;

use crate::views::MainWindow;
use adw::{Application, ColorScheme, prelude::*};
use glib::{ExitCode, clone};
use shared::{config::ApplicationTheme, controller::AppController, info};
use std::{cell::RefCell, rc::Rc};

pub fn run() -> ExitCode {
    let controller = Rc::new(RefCell::new(AppController::default()));
    gio::resources_register_include!("org.nickvision.application.gresource").unwrap();
    let app: Application = Application::builder().application_id(info::APP_ID).build();
    app.connect_startup(clone!(
        #[strong]
        controller,
        move |app| {
            app.style_manager()
                .set_color_scheme(match controller.borrow().theme() {
                    ApplicationTheme::Light => ColorScheme::ForceLight,
                    ApplicationTheme::Dark => ColorScheme::ForceDark,
                    _ => ColorScheme::Default,
                });
        }
    ));
    app.connect_activate(move |app| {
        let window = MainWindow::new(app, controller.clone());
        window.present();
    });
    app.run()
}
