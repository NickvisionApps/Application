use adw::{
    ColorScheme, ComboRow, PreferencesDialog, PreferencesGroup, PreferencesPage, StyleManager,
    SwitchRow, prelude::*, subclass::prelude::*,
};
use glib::{Object, clone};
use gtk::{Image, StringList};
use shared::{config::ApplicationTheme, controller::AppController, translation};
use std::{
    cell::{OnceCell, RefCell},
    rc::Rc,
};

mod imp {
    use super::*;

    #[derive(Default)]
    pub struct SettingsDialogImpl {
        pub(super) controller: OnceCell<Rc<RefCell<AppController>>>,
        pub(super) theme_row: OnceCell<ComboRow>,
        pub(super) language_row: OnceCell<ComboRow>,
        pub(super) preview_updates_row: OnceCell<SwitchRow>,
    }

    #[glib::object_subclass]
    impl ObjectSubclass for SettingsDialogImpl {
        const NAME: &'static str = "SettingsDialogImpl";
        type Type = SettingsDialog;
        type ParentType = PreferencesDialog;
    }

    impl ObjectImpl for SettingsDialogImpl {}

    impl WidgetImpl for SettingsDialogImpl {}

    impl AdwDialogImpl for SettingsDialogImpl {
        fn closed(&self) {
            self.controller.get().unwrap().borrow().save().unwrap();
        }
    }

    impl PreferencesDialogImpl for SettingsDialogImpl {}
}

glib::wrapper! {
    pub struct SettingsDialog(ObjectSubclass<imp::SettingsDialogImpl>)
        @extends adw::PreferencesDialog, adw::Dialog, gtk::Widget,
        @implements gtk::Accessible, gtk::Buildable, gtk::ConstraintTarget, gtk::Native, gtk::Root, gtk::ShortcutManager;
}

impl SettingsDialog {
    pub fn new(controller: Rc<RefCell<AppController>>) -> Self {
        let this: Self = Object::builder().build();
        this.imp().controller.set(controller).unwrap();
        let controller = this.imp().controller.get().unwrap().borrow();
        let theme_row = ComboRow::builder()
            .title(translation::_g("Theme"))
            .model(&StringList::new(&[
                &translation::_g("Light"),
                &translation::_g("Dark"),
                &translation::_g("System"),
            ]))
            .selected(
                ApplicationTheme::ALL
                    .iter()
                    .position(|theme| theme == controller.theme())
                    .unwrap_or(0) as u32,
            )
            .build();
        theme_row.add_prefix(&Image::builder().icon_name("dark-mode-symbolic").build());
        theme_row.connect_selected_notify(clone!(
            #[strong]
            this,
            move |_| {
                this.update_configuration();
            }
        ));
        let language_row = ComboRow::builder()
            .title(translation::_g("Translation Language"))
            .subtitle(translation::_g(
                "An application restart is required for change to take effect",
            ))
            .model(&StringList::new(
                &translation::available_languages()
                    .iter()
                    .map(String::as_str)
                    .collect::<Vec<&str>>(),
            ))
            .selected(
                translation::available_languages()
                    .iter()
                    .position(|language| language == translation::language())
                    .unwrap_or(0) as u32,
            )
            .build();
        language_row.add_prefix(&Image::builder().icon_name("translate-symbolic").build());
        language_row.connect_selected_notify(clone!(
            #[strong]
            this,
            move |_| {
                this.update_configuration();
            }
        ));
        let user_interface_group = PreferencesGroup::builder()
            .title(translation::_g("User Interface"))
            .build();
        user_interface_group.add(&theme_row);
        user_interface_group.add(&language_row);
        let general_page = PreferencesPage::builder()
            .title(translation::_g("General"))
            .icon_name("settings-symbolic")
            .build();
        general_page.add(&user_interface_group);
        let preview_updates_row = SwitchRow::builder()
            .title(translation::_g("Allow Preview Updates"))
            .active(controller.allow_preview_updates())
            .build();
        preview_updates_row.add_prefix(
            &Image::builder()
                .icon_name("software-update-available-symbolic")
                .build(),
        );
        preview_updates_row.connect_active_notify(clone!(
            #[strong]
            this,
            move |_| {
                this.update_configuration();
            }
        ));
        let updates_group = PreferencesGroup::builder()
            .title(translation::_g("Updates"))
            .build();
        updates_group.add(&preview_updates_row);
        let advanced_page = PreferencesPage::builder()
            .title(translation::_g("Advanced"))
            .icon_name("wrench-wide-symbolic")
            .build();
        advanced_page.add(&updates_group);
        this.imp().theme_row.set(theme_row).unwrap();
        this.imp().language_row.set(language_row).unwrap();
        this.imp()
            .preview_updates_row
            .set(preview_updates_row)
            .unwrap();
        this.set_title(&translation::_g("Preferences"));
        this.set_content_width(600);
        this.set_content_height(600);
        this.set_search_enabled(true);
        this.add(&general_page);
        this.add(&advanced_page);
        drop(controller);
        this
    }

    fn update_configuration(&self) {
        let theme = ApplicationTheme::ALL
            .get(self.imp().theme_row.get().unwrap().selected() as usize)
            .cloned()
            .unwrap_or_default();
        StyleManager::default().set_color_scheme(match theme {
            ApplicationTheme::Light => ColorScheme::ForceLight,
            ApplicationTheme::Dark => ColorScheme::ForceDark,
            ApplicationTheme::System => ColorScheme::Default,
        });
        let mut controller = self.imp().controller.get().unwrap().borrow_mut();
        controller.set_theme(theme);
        controller.set_translation_language(
            translation::available_languages()
                .get(self.imp().language_row.get().unwrap().selected() as usize)
                .cloned()
                .unwrap_or_default(),
        );
        controller
            .set_allow_preview_updates(self.imp().preview_updates_row.get().unwrap().is_active());
    }
}
