use adw::{
    Application, ApplicationWindow, ButtonContent, HeaderBar, StatusPage, ToastOverlay,
    ToolbarView, ViewStack, WindowTitle, prelude::*, subclass::prelude::*,
};
use gio::{ActionEntry, Menu};
use glib::Object;
use gtk::{Align, ArrowType, Button, MenuButton};
use shared::AppState;
use std::{cell::OnceCell, sync::Arc};

mod imp {
    use super::*;
    use adw::ToastOverlay;

    #[derive(Default)]
    pub struct MainWindowImpl {
        pub(super) state: OnceCell<Arc<AppState>>,
        pub(super) toast_overlay: OnceCell<ToastOverlay>,
        pub(super) view_stack: OnceCell<ViewStack>,
        pub(super) folder_page: OnceCell<StatusPage>,
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
        let state = self.imp().state.get().unwrap();
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
        header_bar.pack_start(
            &Button::builder()
                .action_name("win.open_folder")
                .tooltip_text(state.translator()._g("Open Folder (Ctrl+O)"))
                .child(
                    &ButtonContent::builder()
                        .label(state.translator()._g("Open"))
                        .icon_name("folder-open-symbolic")
                        .build(),
                )
                .build(),
        );
        header_bar.pack_start(
            &Button::builder()
                .action_name("win.open_folder")
                .tooltip_text(state.translator()._g("Close Folder (Ctrl+W)"))
                .icon_name("window-close-symbolic")
                .build(),
        );
        header_bar.pack_end(
            &MenuButton::builder()
                .primary(true)
                .direction(ArrowType::None)
                .tooltip_text(state.translator()._g("Main Menu"))
                .menu_model(&main_menu)
                .build(),
        );
        let view_stack = ViewStack::new();
        view_stack.add(
            &StatusPage::builder()
                .icon_name("org.nickvision.application")
                .description("Open a folder to get started")
                .child(
                    &Button::builder()
                        .action_name("win.open_folder")
                        .tooltip_text(state.translator()._g("Open Folder (Ctrl+O)"))
                        .halign(Align::Center)
                        .label(state.translator()._g("Open"))
                        .css_classes(["pill", "suggested-action"])
                        .build(),
                )
                .build(),
        );
        let folder_page = view_stack
            .add(
                &StatusPage::builder()
                    .icon_name("folder-documents-symbolic")
                    .css_classes(["compact"])
                    .build(),
            )
            .child()
            .downcast::<StatusPage>()
            .unwrap();
        let toast_overlay = ToastOverlay::new();
        toast_overlay.set_hexpand(true);
        toast_overlay.set_vexpand(true);
        toast_overlay.set_child(Some(&view_stack));
        let toolbar_view = ToolbarView::new();
        toolbar_view.add_top_bar(&header_bar);
        toolbar_view.set_content(Some(&toast_overlay));
        let action_open_folder = ActionEntry::builder("open_folder")
            .activate(|window: &Self, _, _| {
                window.open_folder();
            })
            .build();
        let action_close_folder = ActionEntry::builder("close_folder")
            .activate(|window: &Self, _, _| {
                window.close_folder();
            })
            .build();
        let action_preferences = ActionEntry::builder("preferences")
            .activate(|window: &Self, _, _| {
                window.preferences();
            })
            .build();
        let action_shortcuts = ActionEntry::builder("shortcuts")
            .activate(|window: &Self, _, _| {
                window.shortcuts();
            })
            .build();
        let action_about = ActionEntry::builder("about")
            .activate(|window: &Self, _, _| {
                window.about();
            })
            .build();
        let action_quit = ActionEntry::builder("quit")
            .activate(|window: &Self, _, _| {
                window.quit();
            })
            .build();
        self.imp().toast_overlay.set(toast_overlay).unwrap();
        self.imp().view_stack.set(view_stack).unwrap();
        self.imp().folder_page.set(folder_page).unwrap();
        self.set_size_request(360, 200);
        self.set_default_size(geometry.width() as i32, geometry.height() as i32);
        self.set_content(Some(&toolbar_view));
        self.add_action_entries([
            action_open_folder,
            action_close_folder,
            action_preferences,
            action_shortcuts,
            action_about,
            action_quit,
        ]);
        self.application()
            .unwrap()
            .set_accels_for_action("win.open_folder", &["<Primary>O"]);
        self.application()
            .unwrap()
            .set_accels_for_action("win.close_folder", &["<Primary>W"]);
        self.application()
            .unwrap()
            .set_accels_for_action("win.preferences", &["<Primary>comma"]);
        self.application()
            .unwrap()
            .set_accels_for_action("win.shortcuts", &["<Primary>question"]);
        self.application()
            .unwrap()
            .set_accels_for_action("win.about", &["F1"]);
        self.application()
            .unwrap()
            .set_accels_for_action("win.quit", &["<Primary>Q"]);
    }

    fn open_folder(&self) {}

    fn close_folder(&self) {}

    fn preferences(&self) {}

    fn shortcuts(&self) {}

    fn about(&self) {}

    fn quit(&self) {
        self.close();
    }
}
