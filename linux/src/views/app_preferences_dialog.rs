use adw::{PreferencesDialog, prelude::*, subclass::prelude::*};
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
    }

    #[glib::object_subclass]
    impl ObjectSubclass for AppPreferencesDialogImpl {
        const NAME: &'static str = "AppPreferencesDialogImpl";
        type Type = AppPreferencesDialog;
        type ParentType = PreferencesDialog;
    }

    impl ObjectImpl for AppPreferencesDialogImpl {}

    impl WidgetImpl for AppPreferencesDialogImpl {}

    impl AdwDialogImpl for AppPreferencesDialogImpl {}

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
        self.set_title(&state_ref.translator()._g("Preferences"));
        self.set_content_width(600);
        self.set_content_height(600);
        self.set_search_enabled(true);
    }
}
