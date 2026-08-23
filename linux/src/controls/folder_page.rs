use adw::{StatusPage, subclass::prelude::*};
use glib::Object;
use gtk::{BinLayout, Widget, prelude::*};
use shared::_nf;
use std::{cell::OnceCell, path::Path};

mod imp {
    use super::*;

    #[derive(Default)]
    pub struct FolderPageImp {
        pub(super) status_page: OnceCell<StatusPage>,
    }

    #[glib::object_subclass]
    impl ObjectSubclass for FolderPageImp {
        const NAME: &'static str = "FolderPageImp";
        type Type = FolderPage;
        type ParentType = Widget;
    }

    impl ObjectImpl for FolderPageImp {}

    impl WidgetImpl for FolderPageImp {}

    impl Drop for FolderPageImp {
        fn drop(&mut self) {
            self.status_page.get().unwrap().unparent();
        }
    }
}

glib::wrapper! {
    pub struct FolderPage(ObjectSubclass<imp::FolderPageImp>)
        @extends gtk::Widget,
        @implements gtk::Accessible, gtk::Buildable, gtk::ConstraintTarget, gtk::Native, gtk::Root;
}

impl FolderPage {
    pub fn new() -> Self {
        let this: Self = Object::builder().build();
        let status_page = StatusPage::builder()
            .icon_name("folder-documents-symbolic")
            .css_classes(["compact"])
            .build();
        status_page.set_parent(&this);
        this.imp().status_page.set(status_page).unwrap();
        this.set_layout_manager(Some(BinLayout::new()));
        this
    }

    pub fn page_name() -> &'static str {
        "folder"
    }

    pub fn show_folder(&self, path: &Path, file_count: u64) {
        self.imp()
            .status_page
            .get()
            .unwrap()
            .set_title(path.to_str().unwrap());
        self.imp()
            .status_page
            .get()
            .unwrap()
            .set_description(Some(&_nf(
                "{0} file",
                "{0} files",
                file_count,
                &[file_count.to_string()],
            )));
    }
}
