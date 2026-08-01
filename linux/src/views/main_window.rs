use adw::{
    Application, ApplicationWindow, HeaderBar, ToolbarView, WindowTitle, prelude::*,
    subclass::prelude::*,
};
use gio::{ActionEntry, Menu};
use glib::Object;
use gtk::{ArrowType, MenuButton};
use shared::AppState;
use std::{cell::OnceCell, sync::Arc};

mod imp {
    use super::*;

    #[derive(Default)]
    pub struct MainWindowImpl {
        pub(super) state: OnceCell<Arc<AppState>>,
    }

    #[glib::object_subclass]
    impl ObjectSubclass for MainWindowImpl {
        const NAME: &'static str = "MainWindowImpl";
        type Type = MainWindow;
        type ParentType = ApplicationWindow;
    }

    impl ObjectImpl for MainWindowImpl {}

    impl WidgetImpl for MainWindowImpl {}

    impl WindowImpl for MainWindowImpl {}

    impl ApplicationWindowImpl for MainWindowImpl {}

    impl AdwApplicationWindowImpl for MainWindowImpl {}
}

glib::wrapper! {
    pub struct MainWindow(ObjectSubclass<imp::MainWindowImpl>)
        @extends adw::ApplicationWindow, gtk::ApplicationWindow, gtk::Window, gtk::Widget,
        @implements gio::ActionGroup, gio::ActionMap, gtk::Accessible, gtk::Buildable, gtk::ConstraintTarget, gtk::Native, gtk::Root, gtk::ShortcutManager;
}

impl MainWindow {
    pub fn new(app: &Application, state: Arc<AppState>) -> Self {
        let this: Self = Object::builder().property("application", app).build();
        this.imp().state.set(state).unwrap();
        this.setup_ui();
        this
    }

    fn setup_ui(&self) {
        let state = &self.imp().state.get().unwrap();
        let geometry = state.configuration().window_geometry();
        let main_menu = Menu::new();
        main_menu.append(
            Some(&state.translator()._g("Preferences")),
            Some("win.preferences"),
        );
        main_menu.append(
            Some(&state.translator()._g("Keyboard Shortcuts")),
            Some("win.shortcuts"),
        );
        main_menu.append(
            Some(&state.translator()._g("About Application")),
            Some("win.about"),
        );
        let header_bar = HeaderBar::builder()
            .title_widget(
                &WindowTitle::builder()
                    .title(state.translator()._g("Application"))
                    .build(),
            )
            .build();
        header_bar.pack_end(
            &MenuButton::builder()
                .primary(true)
                .direction(ArrowType::None)
                .tooltip_text(state.translator()._g("Main Menu"))
                .menu_model(&main_menu)
                .build(),
        );
        let toolbar_view = ToolbarView::new();
        toolbar_view.add_top_bar(&header_bar);
        let action_preferences = ActionEntry::builder("preferences")
            .activate(|window: &Self, _, _| {})
            .build();
        let action_shortcuts = ActionEntry::builder("shortcuts")
            .activate(|window: &Self, _, _| {})
            .build();
        let action_about = ActionEntry::builder("about")
            .activate(|window: &Self, _, _| {})
            .build();
        self.set_size_request(360, 200);
        self.set_default_size(geometry.width() as i32, geometry.height() as i32);
        self.set_content(Some(&toolbar_view));
        self.add_action_entries([action_preferences, action_shortcuts, action_about])
    }
}
