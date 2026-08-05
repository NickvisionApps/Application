use adw::{
    ColorScheme, ComboRow, PreferencesDialog, PreferencesGroup, PreferencesPage, StyleManager,
    SwitchRow, prelude::*, subclass::prelude::*,
};
use glib::{Object, clone};
use gtk::{Image, StringList};
use shared::{AppState, ApplicationTheme, Translator};
use std::{
    cell::{OnceCell, RefCell},
    rc::Rc,
};

mod imp {
    use super::*;

    #[derive(Default)]
    pub struct SettingsDialogImpl {
        pub(super) state: OnceCell<Rc<RefCell<AppState>>>,
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
            let state_ref = self.state.get().unwrap().borrow();
            state_ref.configuration().save().unwrap();
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
    pub fn new(state: Rc<RefCell<AppState>>) -> Self {
        let this: Self = Object::builder().build();
        this.imp().state.set(state).unwrap();
        this.setup_ui();
        this
    }

    fn setup_ui(&self) {
        let state_ref = self.imp().state.get().unwrap().borrow();
        let theme_row = ComboRow::builder()
            .title(state_ref.translator()._g("Theme"))
            .model(&StringList::new(&[
                &state_ref.translator()._g("Light"),
                &state_ref.translator()._g("Dark"),
                &state_ref.translator()._g("System"),
            ]))
            .selected(
                ApplicationTheme::ALL
                    .iter()
                    .position(|theme| theme == state_ref.configuration().theme())
                    .unwrap_or(0) as u32,
            )
            .build();
        theme_row.add_prefix(&Image::builder().icon_name("dark-mode-symbolic").build());
        theme_row.connect_selected_notify(clone!(
            #[strong(rename_to = dialog)]
            self,
            move |_| {
                dialog.update_configuration();
            }
        ));
        let language_row = ComboRow::builder()
            .title(state_ref.translator()._g("Translation Language"))
            .subtitle(
                state_ref
                    .translator()
                    ._g("An application restart is required for change to take effect"),
            )
            .model(&StringList::new(
                &Translator::available_languages()
                    .iter()
                    .map(String::as_str)
                    .collect::<Vec<&str>>(),
            ))
            .selected(
                Translator::available_languages()
                    .iter()
                    .position(|language| language == state_ref.translator().language())
                    .unwrap_or(0) as u32,
            )
            .build();
        language_row.add_prefix(&Image::builder().icon_name("translate-symbolic").build());
        language_row.connect_selected_notify(clone!(
            #[strong(rename_to = dialog)]
            self,
            move |_| {
                dialog.update_configuration();
            }
        ));
        let user_interface_group = PreferencesGroup::builder()
            .title(state_ref.translator()._g("User Interface"))
            .build();
        user_interface_group.add(&theme_row);
        user_interface_group.add(&language_row);
        let general_page = PreferencesPage::builder()
            .title(state_ref.translator()._g("General"))
            .icon_name("settings-symbolic")
            .build();
        general_page.add(&user_interface_group);
        let preview_updates_row = SwitchRow::builder()
            .title(state_ref.translator()._g("Allow Preview Updates"))
            .active(state_ref.configuration().allow_preview_updates())
            .build();
        preview_updates_row.add_prefix(
            &Image::builder()
                .icon_name("software-update-available-symbolic")
                .build(),
        );
        preview_updates_row.connect_active_notify(clone!(
            #[strong(rename_to = dialog)]
            self,
            move |_| {
                dialog.update_configuration();
            }
        ));
        let updates_group = PreferencesGroup::builder()
            .title(state_ref.translator()._g("Updates"))
            .build();
        updates_group.add(&preview_updates_row);
        let advanced_page = PreferencesPage::builder()
            .title(state_ref.translator()._g("Advanced"))
            .icon_name("wrench-wide-symbolic")
            .build();
        advanced_page.add(&updates_group);
        self.imp().theme_row.set(theme_row).unwrap();
        self.imp().language_row.set(language_row).unwrap();
        self.imp()
            .preview_updates_row
            .set(preview_updates_row)
            .unwrap();
        self.set_title(&state_ref.translator()._g("Preferences"));
        self.set_content_width(600);
        self.set_content_height(600);
        self.set_search_enabled(true);
        self.add(&general_page);
        self.add(&advanced_page);
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
        let mut state_ref = self.imp().state.get().unwrap().borrow_mut();
        let configuration = state_ref.configuration_mut();
        configuration.set_theme(theme);
        configuration.set_translation_language(
            Translator::available_languages()
                .get(self.imp().language_row.get().unwrap().selected() as usize)
                .cloned()
                .unwrap_or_default(),
        );
        configuration
            .set_allow_preview_updates(self.imp().preview_updates_row.get().unwrap().is_active());
    }
}
