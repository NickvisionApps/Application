use crate::BoxedAppState;
use adw::{
    Application, ApplicationWindow, HeaderBar, ToolbarView, WindowTitle, prelude::*,
    subclass::prelude::*,
};
use glib::{Object, Properties, object::ObjectExt};
use shared::AppState;
use std::{cell::OnceCell, sync::Arc};

mod imp {
    use super::*;

    #[derive(Default, Properties)]
    #[properties(wrapper_type = super::MainWindow)]
    pub struct MainWindow {
        #[property(get, set, construct_only)]
        state: OnceCell<BoxedAppState>,
    }

    #[glib::object_subclass]
    impl ObjectSubclass for MainWindow {
        const NAME: &'static str = "ApplicationMainWindow";
        type Type = super::MainWindow;
        type ParentType = ApplicationWindow;
    }

    #[glib::derived_properties]
    impl ObjectImpl for MainWindow {
        fn constructed(&self) {
            self.parent_constructed();
            let obj = self.obj();
            let state = &obj.imp().state.get().unwrap().0;
            let geometry = state.configuration().window_geometry();
            let toolbar_view = ToolbarView::new();
            let header_bar = HeaderBar::builder().title_widget(
                &WindowTitle::builder()
                    .title(state.translator()._g("Application"))
                    .build(),
            );
            toolbar_view.add_top_bar(&header_bar.build());
            obj.set_size_request(360, 200);
            obj.set_default_size(geometry.width() as i32, geometry.height() as i32);
            obj.set_content(Some(&toolbar_view));
        }
    }

    impl WidgetImpl for MainWindow {}

    impl WindowImpl for MainWindow {}

    impl ApplicationWindowImpl for MainWindow {}

    impl AdwApplicationWindowImpl for MainWindow {}
}

glib::wrapper! {
    pub struct MainWindow(ObjectSubclass<imp::MainWindow>)
        @extends adw::ApplicationWindow, gtk::ApplicationWindow, gtk::Window, gtk::Widget,
        @implements gio::ActionGroup, gio::ActionMap, gtk::Accessible, gtk::Buildable, gtk::ConstraintTarget, gtk::Native, gtk::Root, gtk::ShortcutManager;
}

impl MainWindow {
    pub fn new(app: &Application, state: Arc<AppState>) -> Self {
        Object::builder()
            .property("application", app)
            .property("state", BoxedAppState(state))
            .build()
    }
}
