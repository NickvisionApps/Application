use crate::views::SettingsDialog;
use adw::{
    AboutDialog, Application, ApplicationWindow, ButtonContent, HeaderBar, ShortcutsDialog,
    ShortcutsItem, ShortcutsSection, StatusPage, Toast, ToastOverlay, ToolbarView, ViewStack,
    WindowTitle, prelude::*, subclass::prelude::*,
};
use gio::{ActionEntry, Cancellable, Menu};
use glib::{Object, Propagation, clone};
use gtk::{Align, ArrowType, Button, FileDialog, License, MenuButton};
use markdown::to_html;
use shared::{
    APP_ARTISTS, APP_CHANGELOG, APP_DESCRIPTION, APP_DESIGNERS, APP_DEVELOPERS, APP_DISCUSSION_URL,
    APP_ENGLISH_SHORT_NAME, APP_ID, APP_ISSUES_URL, APP_REPO_URL, AppController, WindowGeometry,
    app_version, debugging_information,
};
use std::{
    cell::{OnceCell, RefCell},
    rc::Rc,
};

mod imp {
    use super::*;

    #[derive(Default)]
    pub struct MainWindowImpl {
        pub(super) controller: OnceCell<Rc<RefCell<AppController>>>,
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

    impl WindowImpl for MainWindowImpl {
        fn close_request(&self) -> Propagation {
            let mut controller = self.controller.get().unwrap().borrow_mut();
            if !controller.can_close() {
                return Propagation::Stop;
            }
            let configuration = controller.configuration_mut();
            if self.obj().is_maximized() {
                configuration
                    .set_window_geometry(WindowGeometry::builder().is_maximized(true).build());
            } else {
                configuration.set_window_geometry(
                    WindowGeometry::builder()
                        .width(self.obj().default_width() as u64)
                        .height(self.obj().default_height() as u64)
                        .build(),
                );
            }
            configuration.save().unwrap();
            Propagation::Proceed
        }
    }

    impl ApplicationWindowImpl for MainWindowImpl {}

    impl AdwApplicationWindowImpl for MainWindowImpl {}
}

glib::wrapper! {
    pub struct MainWindow(ObjectSubclass<imp::MainWindowImpl>)
        @extends adw::ApplicationWindow, gtk::ApplicationWindow, gtk::Window, gtk::Widget,
        @implements gio::ActionGroup, gio::ActionMap, gtk::Accessible, gtk::Buildable, gtk::ConstraintTarget, gtk::Native, gtk::Root, gtk::ShortcutManager;
}

impl MainWindow {
    pub fn new(app: &Application, controller: Rc<RefCell<AppController>>) -> Self {
        let this: Self = Object::builder().property("application", app).build();
        this.imp().controller.set(controller).unwrap();
        this.setup_ui();
        this
    }

    fn setup_ui(&self) {
        let controller = self.imp().controller.get().unwrap().borrow();
        let geometry = controller.configuration().window_geometry();
        let main_menu = Menu::new();
        main_menu.append(
            Some(&controller.translator()._g("Preferences")),
            Some("win.preferences"),
        );
        main_menu.append(
            Some(&controller.translator()._g("Keyboard Shortcuts")),
            Some("win.shortcuts"),
        );
        main_menu.append(
            Some(&controller.translator()._g("About Application")),
            Some("win.about"),
        );
        let header_bar = HeaderBar::builder()
            .title_widget(
                &WindowTitle::builder()
                    .title(controller.translator()._g("Application"))
                    .build(),
            )
            .build();
        header_bar.pack_start(
            &Button::builder()
                .action_name("win.open_folder")
                .tooltip_text(controller.translator()._g("Open Folder (Ctrl+O)"))
                .child(
                    &ButtonContent::builder()
                        .label(controller.translator()._g("Open"))
                        .icon_name("folder-open-symbolic")
                        .build(),
                )
                .build(),
        );
        header_bar.pack_start(
            &Button::builder()
                .action_name("win.close_folder")
                .tooltip_text(controller.translator()._g("Close Folder (Ctrl+W)"))
                .icon_name("window-close-symbolic")
                .build(),
        );
        header_bar.pack_end(
            &MenuButton::builder()
                .primary(true)
                .direction(ArrowType::None)
                .tooltip_text(controller.translator()._g("Main Menu"))
                .menu_model(&main_menu)
                .build(),
        );
        let view_stack = ViewStack::new();
        view_stack.add_named(
            &StatusPage::builder()
                .icon_name("org.nickvision.application")
                .title(controller.greeting())
                .description("Open a folder to get started")
                .child(
                    &Button::builder()
                        .action_name("win.open_folder")
                        .tooltip_text(controller.translator()._g("Open Folder (Ctrl+O)"))
                        .halign(Align::Center)
                        .label(controller.translator()._g("Open"))
                        .css_classes(["pill", "suggested-action"])
                        .build(),
                )
                .build(),
            Some("home"),
        );
        let folder_page = StatusPage::builder()
            .icon_name("folder-documents-symbolic")
            .css_classes(["compact"])
            .build();
        view_stack.add_named(&folder_page, Some("folder"));
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

    fn about(&self) {
        let controller = self.imp().controller.get().unwrap().borrow();
        let about_dialog = AboutDialog::builder()
            .application_name(APP_ENGLISH_SHORT_NAME)
            .application_icon(if app_version().pre.is_empty() {
                APP_ID.to_string()
            } else {
                format!("{}-devel", APP_ID)
            })
            .developer_name("Nickvision")
            .version(app_version().to_string())
            .release_notes(to_html(APP_CHANGELOG))
            .debug_info(debugging_information())
            .comments(APP_DESCRIPTION)
            .license_type(License::MitX11)
            .copyright("© Nickvision 2021-2026")
            .website("https://nickvision.org")
            .issue_url(APP_ISSUES_URL)
            .support_url(APP_DISCUSSION_URL)
            .build();
        let artists: Vec<String> = APP_ARTISTS
            .iter()
            .map(|(x, y)| format!("{} {}", x, y))
            .collect();
        let designers: Vec<String> = APP_DESIGNERS
            .iter()
            .map(|(x, y)| format!("{} {}", x, y))
            .collect();
        let developers: Vec<String> = APP_DEVELOPERS
            .iter()
            .map(|(x, y)| format!("{} {}", x, y))
            .collect();
        let translation_credits = controller.translator()._g("translation-credits");
        about_dialog.add_link(&controller.translator()._g("GitHub Repo"), APP_REPO_URL);
        about_dialog.set_artists(&artists.iter().map(|x| x.as_str()).collect::<Vec<&str>>());
        about_dialog.set_designers(&designers.iter().map(|x| x.as_str()).collect::<Vec<&str>>());
        about_dialog.set_developers(&developers.iter().map(|x| x.as_str()).collect::<Vec<&str>>());
        if !translation_credits.is_empty() && translation_credits != "translation-credits" {
            about_dialog.set_translator_credits(&translation_credits);
        }
        about_dialog.present(Some(self));
    }

    fn close_folder(&self) {
        let mut controller = self.imp().controller.get().unwrap().borrow_mut();
        controller.folder_browser_mut().close();
        self.imp()
            .view_stack
            .get()
            .unwrap()
            .set_visible_child_name("home");
        self.imp().toast_overlay.get().unwrap().add_toast(
            Toast::builder()
                .use_markup(false)
                .title(controller.translator()._g("Folder closed"))
                .build(),
        );
    }

    fn open_folder(&self) {
        let controller = self.imp().controller.get().unwrap().borrow();
        let file_dialog = FileDialog::builder()
            .title(controller.translator()._g("Open Folder"))
            .build();
        file_dialog.select_folder(
            Some(self),
            Cancellable::NONE,
            clone!(
                #[strong(rename_to = window)]
                self,
                move |res| {
                    let mut controller = window.imp().controller.get().unwrap().borrow_mut();
                    if let Ok(file) = res {
                        if let Err(error) =
                            controller.folder_browser_mut().open(file.path().unwrap())
                        {
                            window.imp().toast_overlay.get().unwrap().add_toast(
                                Toast::builder()
                                    .use_markup(false)
                                    .title(
                                        controller
                                            .translator()
                                            ._f("Unable to open folder: {0}", &[error.to_string()]),
                                    )
                                    .build(),
                            );
                        } else {
                            window
                                .imp()
                                .view_stack
                                .get()
                                .unwrap()
                                .set_visible_child_name("folder");
                            window
                                .imp()
                                .folder_page
                                .get()
                                .unwrap()
                                .set_title(controller.folder_browser().path().to_str().unwrap());
                            window
                                .imp()
                                .folder_page
                                .get()
                                .unwrap()
                                .set_description(Some(&controller.translator()._nf(
                                    "{0} file",
                                    "{0} files",
                                    controller.folder_browser().files().len() as u64,
                                    &[controller.folder_browser().files().len().to_string()],
                                )))
                        }
                    }
                }
            ),
        )
    }

    fn preferences(&self) {
        let dialog = SettingsDialog::new(self.imp().controller.get().unwrap().clone());
        dialog.present(Some(self));
    }

    fn quit(&self) {
        self.close();
    }

    fn shortcuts(&self) {
        let controller = self.imp().controller.get().unwrap().borrow();
        let app_section = ShortcutsSection::new(Some(&controller.translator()._g("App")));
        app_section.add(ShortcutsItem::new(
            &controller.translator()._g("Preferences"),
            "<Primary>comma",
        ));
        app_section.add(ShortcutsItem::new(
            &controller.translator()._g("Keyboard Shortcuts"),
            "<Primary>question",
        ));
        app_section.add(ShortcutsItem::new(
            &controller.translator()._g("About Application"),
            "F1",
        ));
        app_section.add(ShortcutsItem::new(
            &controller.translator()._g("Quit"),
            "<Primary>q",
        ));
        let folder_section = ShortcutsSection::new(Some(&controller.translator()._g("Folder")));
        folder_section.add(ShortcutsItem::new(
            &controller.translator()._g("Open Folder"),
            "<Primary>o",
        ));
        folder_section.add(ShortcutsItem::new(
            &controller.translator()._g("Close Folder"),
            "<Primary>w",
        ));
        let dialog = ShortcutsDialog::new();
        dialog.add(app_section);
        dialog.add(folder_section);
        dialog.present(Some(self));
    }
}
