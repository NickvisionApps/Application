use adw::{StatusPage, subclass::prelude::*};
use glib::Object;
use gtk::{Align, BinLayout, Button, Widget, prelude::*};
use shared::_g;
use std::cell::OnceCell;
mod imp {
    use super::*;

    #[derive(Default)]
    pub struct HomePageImp {
        pub(super) status_page: OnceCell<StatusPage>,
    }

    #[glib::object_subclass]
    impl ObjectSubclass for HomePageImp {
        const NAME: &'static str = "HomePageImp";
        type Type = HomePage;
        type ParentType = Widget;
    }

    impl ObjectImpl for HomePageImp {}

    impl WidgetImpl for HomePageImp {}

    impl Drop for HomePageImp {
        fn drop(&mut self) {
            self.status_page.get().unwrap().unparent();
        }
    }
}

glib::wrapper! {
    pub struct HomePage(ObjectSubclass<imp::HomePageImp>)
        @extends gtk::Widget,
        @implements gtk::Accessible, gtk::Buildable, gtk::ConstraintTarget, gtk::Native, gtk::Root;
}

impl HomePage {
    pub fn new() -> Self {
        let this: Self = Object::builder().build();
        let status_page = StatusPage::builder()
            .icon_name("org.nickvision.application")
            .description("Open a folder to get started")
            .child(
                &Button::builder()
                    .action_name("win.open_folder")
                    .tooltip_text(_g("Open Folder (Ctrl+O)"))
                    .halign(Align::Center)
                    .label(_g("Open"))
                    .css_classes(["pill", "suggested-action"])
                    .build(),
            )
            .build();
        status_page.set_parent(&this);
        this.imp().status_page.set(status_page).unwrap();
        this.set_layout_manager(Some(BinLayout::new()));
        this
    }

    pub fn page_name() -> &'static str {
        "home"
    }

    pub fn set_greeting(&self, greeting: &str) {
        self.imp().status_page.get().unwrap().set_title(greeting);
    }
}
