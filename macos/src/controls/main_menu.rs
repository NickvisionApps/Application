use crate::helpers::EasyMenu;
use crate::views::{MainWindow, SettingsDialog};
use objc2::rc::Retained;
use objc2::runtime::AnyObject;
use objc2::{
    ClassType, DefinedClass, MainThreadMarker, MainThreadOnly, define_class, msg_send, sel,
};
use objc2_app_kit::{NSAlert, NSApplication, NSEventModifierFlags, NSMenu, NSMenuItem};
use objc2_foundation::{NSObjectProtocol, NSString, ns_string};
use shared::{controller::AppController, info, translation};
use std::{
    cell::{OnceCell, RefCell},
    rc::Rc,
};

#[derive(Debug)]
pub struct MainMenuState {
    controller: Rc<RefCell<AppController>>,
    window: Retained<MainWindow>,
    settings_dialog: OnceCell<Retained<SettingsDialog>>,
}

define_class!(
    #[derive(Debug)]
    #[unsafe(super = NSMenu)]
    #[thread_kind = MainThreadOnly]
    #[ivars = MainMenuState]
    pub struct MainMenu;

    impl MainMenu {
        #[unsafe(method(checkForUpdates:))]
        fn check_for_updates(&self, _sender: Option<&AnyObject>) {
            self.ivars().window.show();
            self.ivars().window.check_for_updates();
        }

        #[unsafe(method(closeFolder:))]
        fn close_folder(&self, _sender: Option<&AnyObject>) {
            self.ivars().window.show();
            self.ivars().window.close_folder();
        }

        #[unsafe(method(openFolder:))]
        fn open_folder(&self, _sender: Option<&AnyObject>) {
            self.ivars().window.show();
            self.ivars().window.open_folder();
        }

        #[unsafe(method(showChangelog:))]
        fn show_changelog(&self, _sender: Option<&AnyObject>) {
            let dialog = NSAlert::new(self.mtm());
            dialog.setMessageText(&NSString::from_str(&translation::_f("{0}\n\nWhat's New?", &[info::APP_DESCRIPTION])));
            dialog.setInformativeText(&NSString::from_str(info::APP_CHANGELOG));
            dialog.runModal();
        }

        #[unsafe(method(showCredits:))]
        fn show_credits(&self, _sender: Option<&AnyObject>) {
            let dialog = NSAlert::new(self.mtm());
            let translation_credits = translation::_g("translation-credits");
            dialog.setMessageText(&NSString::from_str(&translation::_g("Credits")));
            if !translation_credits.is_empty() && translation_credits != "translation-credits" {
                dialog.setInformativeText(&NSString::from_str(&translation::_f("Developers:\n{0}\n\nDesigners:\n{1}\n\nArtists:\n{2}\n\nTranslators:\n{3}", &[info::app_developer_names(), info::app_designer_names(), info::app_artist_names(), translation_credits])));
            } else {
                dialog.setInformativeText(&NSString::from_str(&translation::_f("Developers:\n{0}\n\nDesigners:\n{1}\n\nArtists:\n{2}", &[info::app_developer_names(), info::app_designer_names(), info::app_artist_names()])));
            }
            dialog.runModal();
        }

        #[unsafe(method(showDebuggingInformation:))]
        fn show_debugging_information(&self, _sender: Option<&AnyObject>) {
            let dialog = NSAlert::new(self.mtm());
            dialog.setMessageText(&NSString::from_str(&translation::_g("Debugging Information")));
            dialog.setInformativeText(&NSString::from_str(&info::debugging_information()));
            dialog.runModal();
        }

        #[unsafe(method(showSettings:))]
        fn show_settings(&self, _sender: Option<&AnyObject>) {
            self.ivars().settings_dialog.get_or_init(|| {
                SettingsDialog::new(self.mtm(), Rc::clone(&self.ivars().controller))
            }).show();
        }
    }

    unsafe impl NSObjectProtocol for MainMenu {}
);

impl MainMenuState {
    pub fn new(controller: Rc<RefCell<AppController>>, window: Retained<MainWindow>) -> Self {
        MainMenuState {
            controller,
            window,
            settings_dialog: OnceCell::default(),
        }
    }
}

impl MainMenu {
    pub fn new(
        mtm: MainThreadMarker,
        controller: Rc<RefCell<AppController>>,
        window: Retained<MainWindow>,
    ) -> Retained<Self> {
        let this = Self::alloc(mtm).set_ivars(MainMenuState::new(controller, window));
        let this: Retained<Self> =
            unsafe { msg_send![super(this), initWithTitle: ns_string!("MainMenu")] };
        let app_menu_item = NSMenuItem::new(mtm);
        let app_menu = NSMenu::initWithTitle(
            NSMenu::alloc(mtm),
            &NSString::from_str(&translation::_g("Application")),
        );
        app_menu.add_item_easy(
            translation::_g("About Application"),
            None,
            "",
            None,
            Some(sel!(orderFrontStandardAboutPanel:)),
        );
        app_menu.add_item_easy(
            translation::_g("Check for Updates"),
            Some("arrow.trianglehead.2.clockwise.rotate.90"),
            "",
            Some(this.as_super().as_super()),
            Some(sel!(checkForUpdates:)),
        );
        app_menu.add_separator_item();
        app_menu.add_item_easy(
            translation::_g("Settings\u{2026}"),
            Some("gearshape"),
            ",",
            Some(this.as_super().as_super()),
            Some(sel!(showSettings:)),
        );
        app_menu.add_separator_item();
        app_menu.add_services_item(translation::_g("Services"));
        app_menu.add_separator_item();
        app_menu.add_item_easy(
            translation::_g("Hide Application"),
            None,
            "h",
            None,
            Some(sel!(hide:)),
        );
        let hide_others_item = app_menu.add_item_easy(
            translation::_g("Hide Others"),
            None,
            "h",
            None,
            Some(sel!(hideOtherApplications:)),
        );
        hide_others_item.setKeyEquivalentModifierMask(
            NSEventModifierFlags::Command | NSEventModifierFlags::Option,
        );
        app_menu.add_item_easy(
            translation::_g("Show All"),
            None,
            "",
            None,
            Some(sel!(unhideAllApplications:)),
        );
        app_menu.add_separator_item();
        app_menu.add_item_easy(
            translation::_g("Quit Application"),
            None,
            "q",
            None,
            Some(sel!(terminate:)),
        );
        app_menu_item.setSubmenu(Some(&app_menu));
        this.addItem(&app_menu_item);
        let file_menu_item = NSMenuItem::new(mtm);
        let file_menu = NSMenu::initWithTitle(
            NSMenu::alloc(mtm),
            &NSString::from_str(&translation::_g("File")),
        );
        file_menu.add_item_easy(
            translation::_g("Open Folder"),
            Some("folder.badge.plus"),
            "o",
            Some(this.as_super().as_super()),
            Some(sel!(openFolder:)),
        );
        file_menu.add_item_easy(
            translation::_g("Close Folder"),
            Some("folder.badge.minus"),
            "W",
            Some(this.as_super().as_super()),
            Some(sel!(closeFolder:)),
        );
        file_menu.add_separator_item();
        file_menu.add_item_easy(
            translation::_g("Close Window"),
            None,
            "w",
            None,
            Some(sel!(performClose:)),
        );
        file_menu_item.setSubmenu(Some(&file_menu));
        this.addItem(&file_menu_item);
        let edit_menu_item = NSMenuItem::new(mtm);
        let edit_menu = NSMenu::initWithTitle(
            NSMenu::alloc(mtm),
            &NSString::from_str(&translation::_g("Edit")),
        );
        edit_menu.add_item_easy(translation::_g("Undo"), None, "z", None, Some(sel!(undo:)));
        edit_menu.add_item_easy(translation::_g("Redo"), None, "Z", None, Some(sel!(redo:)));
        edit_menu.add_separator_item();
        edit_menu.add_item_easy(translation::_g("Cut"), None, "x", None, Some(sel!(cut:)));
        edit_menu.add_item_easy(translation::_g("Copy"), None, "c", None, Some(sel!(copy:)));
        edit_menu.add_item_easy(
            translation::_g("Paste"),
            None,
            "v",
            None,
            Some(sel!(paste:)),
        );
        edit_menu.add_item_easy(
            translation::_g("Select All"),
            None,
            "a",
            None,
            Some(sel!(selectAll:)),
        );
        edit_menu_item.setSubmenu(Some(&edit_menu));
        this.addItem(&edit_menu_item);
        let view_menu_item = NSMenuItem::new(mtm);
        let view_menu = NSMenu::initWithTitle(
            NSMenu::alloc(mtm),
            &NSString::from_str(&translation::_g("View")),
        );
        let full_screen_item = view_menu.add_item_easy(
            translation::_g("Enter Full Screen"),
            None,
            "f",
            None,
            Some(sel!(toggleFullScreen:)),
        );
        full_screen_item.setKeyEquivalentModifierMask(
            NSEventModifierFlags::Command | NSEventModifierFlags::Control,
        );
        view_menu_item.setSubmenu(Some(&view_menu));
        this.addItem(&view_menu_item);
        let window_menu_item = NSMenuItem::new(mtm);
        let window_menu = NSMenu::initWithTitle(
            NSMenu::alloc(mtm),
            &NSString::from_str(&translation::_g("Window")),
        );
        window_menu.add_item_easy(
            translation::_g("Minimize"),
            None,
            "m",
            None,
            Some(sel!(performMiniaturize:)),
        );
        window_menu.add_item_easy(
            translation::_g("Zoom"),
            None,
            "",
            None,
            Some(sel!(performZoom:)),
        );
        window_menu.add_separator_item();
        window_menu.add_item_easy(
            translation::_g("Bring All to Front"),
            None,
            "",
            None,
            Some(sel!(arrangeInFront:)),
        );
        window_menu_item.setSubmenu(Some(&window_menu));
        NSApplication::sharedApplication(mtm).setWindowsMenu(Some(&window_menu));
        this.addItem(&window_menu_item);
        let help_menu_item = NSMenuItem::new(mtm);
        let help_menu = NSMenu::initWithTitle(
            NSMenu::alloc(mtm),
            &NSString::from_str(&translation::_g("Help")),
        );
        help_menu.add_item_easy(
            translation::_g("What's New"),
            Some("newspaper"),
            "",
            Some(this.as_super().as_super()),
            Some(sel!(showChangelog:)),
        );
        help_menu.add_item_easy(
            translation::_g("Credits"),
            Some("person.2"),
            "",
            Some(this.as_super().as_super()),
            Some(sel!(showCredits:)),
        );
        help_menu.add_separator_item();
        help_menu.add_item_easy(
            translation::_g("Debugging Information"),
            Some("exclamationmark.bubble"),
            "",
            Some(this.as_super().as_super()),
            Some(sel!(showDebuggingInformation:)),
        );
        help_menu_item.setSubmenu(Some(&help_menu));
        NSApplication::sharedApplication(mtm).setHelpMenu(Some(&help_menu));
        this.addItem(&help_menu_item);
        this
    }
}
