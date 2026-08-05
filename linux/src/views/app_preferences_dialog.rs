use adw::{
    ComboRow, PreferencesDialog, PreferencesGroup, PreferencesPage, prelude::*,
    subclass::prelude::*,
};
use glib::Object;
use shared::AppState;
use std::{
    cell::{OnceCell, RefCell},
    rc::Rc,
};

mod imp {
    use super::*;

    #[derive(Default)]
    pub struct AppPreferencesDialogImpl {
        pub(super) state: OnceCell<Rc<RefCell<AppState>>>,
        pub(super) theme_row: OnceCell<ComboRow>,
        pub(super) language_row: OnceCell<ComboRow>,
    }

    #[glib::object_subclass]
    impl ObjectSubclass for AppPreferencesDialogImpl {
        const NAME: &'static str = "AppPreferencesDialogImpl";
        type Type = AppPreferencesDialog;
        type ParentType = PreferencesDialog;
    }

    impl ObjectImpl for AppPreferencesDialogImpl {}

    impl WidgetImpl for AppPreferencesDialogImpl {}

    impl AdwDialogImpl for AppPreferencesDialogImpl {
        fn closed(&self) {
            let state_ref = self.state.get().unwrap().borrow();
            state_ref.configuration().save().unwrap();
        }
    }

    impl PreferencesDialogImpl for AppPreferencesDialogImpl {}
}

glib::wrapper! {
    pub struct AppPreferencesDialog(ObjectSubclass<imp::AppPreferencesDialogImpl>)
        @extends adw::PreferencesDialog, adw::Dialog, gtk::Widget,
        @implements gtk::Accessible, gtk::Buildable, gtk::ConstraintTarget, gtk::Native, gtk::Root, gtk::ShortcutManager;
}

impl AppPreferencesDialog {
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
            .build();
        let language_row = ComboRow::builder()
            .title(state_ref.translator()._g("Translation Language"))
            .subtitle(
                state_ref
                    .translator()
                    ._g("An application restart is required for a change to take effect"),
            )
            .build();
        let user_interface_group = PreferencesGroup::builder()
            .title(state_ref.translator()._g("User Interface"))
            .build();
        user_interface_group.add(&theme_row);
        user_interface_group.add(&language_row);
        let general_page = PreferencesPage::builder()
            .title(state_ref.translator()._g("General"))
            .build();
        general_page.add(&user_interface_group);
        self.imp().theme_row.set(theme_row).unwrap();
        self.imp().language_row.set(language_row).unwrap();
        self.set_title(&state_ref.translator()._g("Preferences"));
        self.set_content_width(600);
        self.set_content_height(600);
        self.set_search_enabled(true);
        self.add(&general_page);
    }
}
