use crate::views::{MainWindow, SettingsDialog};
use objc2::rc::Retained;
use objc2::runtime::AnyObject;
use objc2::{
    ClassType, DeclaredClass, MainThreadMarker, MainThreadOnly, define_class, msg_send, sel,
};
use objc2_app_kit::{NSAlert, NSApplication, NSEventModifierFlags, NSImage, NSMenu, NSMenuItem};
use objc2_foundation::{NSObjectProtocol, NSString, ns_string};
use shared::{
    APP_CHANGELOG, APP_DESCRIPTION, AppState, app_artist_names, app_designer_names,
    app_developer_names,
};
use std::cell::RefCell;
use std::rc::Rc;

#[derive(Debug)]
pub struct MainMenuState {
    state: Rc<RefCell<AppState>>,
    window: Retained<MainWindow>,
    settings_dialog: RefCell<Option<Retained<SettingsDialog>>>,
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
            let state_ref = self.ivars().state.borrow();
            let dialog = NSAlert::new(self.mtm());
            dialog.setMessageText(&NSString::from_str(&state_ref.translator()._f("{0}\n\nWhat's New?", &[APP_DESCRIPTION])));
            dialog.setInformativeText(&NSString::from_str(APP_CHANGELOG));
            dialog.runModal();
        }

        #[unsafe(method(showCredits:))]
        fn show_credits(&self, _sender: Option<&AnyObject>) {
            let state_ref = self.ivars().state.borrow();
            let dialog = NSAlert::new(self.mtm());
            dialog.setMessageText(&NSString::from_str(&state_ref.translator()._g("Credits")));
            dialog.setInformativeText(&NSString::from_str(&state_ref.translator()._f("Developers:\n{0}\n\nDesigners:\n{1}\n\nArtists:\n{2}", &[app_developer_names(), app_designer_names(), app_artist_names()])));
            dialog.runModal();
        }

        #[unsafe(method(showSettings:))]
        fn show_settings(&self, _sender: Option<&AnyObject>) {
            let dialog = SettingsDialog::new(self.mtm(), Rc::clone(&self.ivars().state));
            dialog.show();
            *self.ivars().settings_dialog.borrow_mut() = Some(dialog);
        }
    }

    unsafe impl NSObjectProtocol for MainMenu {}
);

impl MainMenuState {
    pub fn new(state: Rc<RefCell<AppState>>, window: Retained<MainWindow>) -> Self {
        MainMenuState {
            state,
            window,
            settings_dialog: RefCell::new(None),
        }
    }
}

impl MainMenu {
    pub fn new(
        mtm: MainThreadMarker,
        state: Rc<RefCell<AppState>>,
        window: Retained<MainWindow>,
    ) -> Retained<Self> {
        let this = Self::alloc(mtm).set_ivars(MainMenuState::new(state, window));
        let this: Retained<Self> =
            unsafe { msg_send![super(this), initWithTitle: ns_string!("MainMenu")] };
        let state_ref = this.ivars().state.borrow();
        let app_menu_item = NSMenuItem::new(mtm);
        let app_menu = NSMenu::initWithTitle(
            NSMenu::alloc(mtm),
            &NSString::from_str(&state_ref.translator()._g("Application")),
        );
        unsafe {
            app_menu.addItemWithTitle_action_keyEquivalent(
                &NSString::from_str(&state_ref.translator()._g("About Application")),
                Some(sel!(orderFrontStandardAboutPanel:)),
                ns_string!(""),
            );
            let updates_item = app_menu.addItemWithTitle_action_keyEquivalent(
                &NSString::from_str(&state_ref.translator()._g("Check for Updates")),
                Some(sel!(checkForUpdates:)),
                ns_string!(""),
            );
            updates_item.setTarget(Some(this.as_super().as_super()));
        }
        app_menu.addItem(&NSMenuItem::separatorItem(mtm));
        let settings_item = unsafe {
            app_menu.addItemWithTitle_action_keyEquivalent(
                &NSString::from_str(&state_ref.translator()._g("Settings\u{2026}")),
                Some(sel!(showSettings:)),
                ns_string!(","),
            )
        };
        unsafe { settings_item.setTarget(Some(this.as_super().as_super())) };
        if let Some(gear_image) = NSImage::imageWithSystemSymbolName_accessibilityDescription(
            ns_string!("gearshape"),
            None,
        ) {
            settings_item.setImage(Some(&gear_image));
        }
        app_menu.addItem(&NSMenuItem::separatorItem(mtm));
        let services_menu_item = unsafe {
            app_menu.addItemWithTitle_action_keyEquivalent(
                &NSString::from_str(&state_ref.translator()._g("Services")),
                None,
                ns_string!(""),
            )
        };
        let services_menu = NSMenu::initWithTitle(
            NSMenu::alloc(mtm),
            &NSString::from_str(&state_ref.translator()._g("Services")),
        );
        services_menu_item.setSubmenu(Some(&services_menu));
        NSApplication::sharedApplication(mtm).setServicesMenu(Some(&services_menu));
        app_menu.addItem(&NSMenuItem::separatorItem(mtm));
        unsafe {
            app_menu.addItemWithTitle_action_keyEquivalent(
                &NSString::from_str(&state_ref.translator()._g("Hide Application")),
                Some(sel!(hide:)),
                ns_string!("h"),
            );
        }
        let hide_others_item = unsafe {
            app_menu.addItemWithTitle_action_keyEquivalent(
                &NSString::from_str(&state_ref.translator()._g("Hide Others")),
                Some(sel!(hideOtherApplications:)),
                ns_string!("h"),
            )
        };
        hide_others_item.setKeyEquivalentModifierMask(
            NSEventModifierFlags::Command | NSEventModifierFlags::Option,
        );
        unsafe {
            app_menu.addItemWithTitle_action_keyEquivalent(
                &NSString::from_str(&state_ref.translator()._g("Show All")),
                Some(sel!(unhideAllApplications:)),
                ns_string!(""),
            );
        }
        app_menu.addItem(&NSMenuItem::separatorItem(mtm));
        unsafe {
            app_menu.addItemWithTitle_action_keyEquivalent(
                &NSString::from_str(&state_ref.translator()._g("Quit Application")),
                Some(sel!(terminate:)),
                ns_string!("q"),
            );
        }
        app_menu_item.setSubmenu(Some(&app_menu));
        this.addItem(&app_menu_item);
        let file_menu_item = NSMenuItem::new(mtm);
        let file_menu = NSMenu::initWithTitle(
            NSMenu::alloc(mtm),
            &NSString::from_str(&state_ref.translator()._g("File")),
        );
        unsafe {
            let open_folder_item = file_menu.addItemWithTitle_action_keyEquivalent(
                &NSString::from_str(&state_ref.translator()._g("Open Folder")),
                Some(sel!(openFolder:)),
                ns_string!("o"),
            );
            let close_folder_item = file_menu.addItemWithTitle_action_keyEquivalent(
                &NSString::from_str(&state_ref.translator()._g("Close Folder")),
                Some(sel!(closeFolder:)),
                ns_string!("W"),
            );
            open_folder_item.setTarget(Some(this.as_super().as_super()));
            close_folder_item.setTarget(Some(this.as_super().as_super()));
        }
        file_menu.addItem(&NSMenuItem::separatorItem(mtm));
        unsafe {
            file_menu.addItemWithTitle_action_keyEquivalent(
                &NSString::from_str(&state_ref.translator()._g("Close Window")),
                Some(sel!(performClose:)),
                ns_string!("w"),
            );
        }
        file_menu_item.setSubmenu(Some(&file_menu));
        this.addItem(&file_menu_item);
        let edit_menu_item = NSMenuItem::new(mtm);
        let edit_menu = NSMenu::initWithTitle(
            NSMenu::alloc(mtm),
            &NSString::from_str(&state_ref.translator()._g("Edit")),
        );
        unsafe {
            edit_menu.addItemWithTitle_action_keyEquivalent(
                &NSString::from_str(&state_ref.translator()._g("Undo")),
                Some(sel!(undo:)),
                ns_string!("z"),
            );
            edit_menu.addItemWithTitle_action_keyEquivalent(
                &NSString::from_str(&state_ref.translator()._g("Redo")),
                Some(sel!(redo:)),
                ns_string!("Z"),
            );
        }
        edit_menu.addItem(&NSMenuItem::separatorItem(mtm));
        unsafe {
            edit_menu.addItemWithTitle_action_keyEquivalent(
                &NSString::from_str(&state_ref.translator()._g("Cut")),
                Some(sel!(cut:)),
                ns_string!("x"),
            );
            edit_menu.addItemWithTitle_action_keyEquivalent(
                &NSString::from_str(&state_ref.translator()._g("Copy")),
                Some(sel!(copy:)),
                ns_string!("c"),
            );
            edit_menu.addItemWithTitle_action_keyEquivalent(
                &NSString::from_str(&state_ref.translator()._g("Paste")),
                Some(sel!(paste:)),
                ns_string!("v"),
            );
            edit_menu.addItemWithTitle_action_keyEquivalent(
                &NSString::from_str(&state_ref.translator()._g("Select All")),
                Some(sel!(selectAll:)),
                ns_string!("a"),
            );
        }
        edit_menu_item.setSubmenu(Some(&edit_menu));
        this.addItem(&edit_menu_item);
        let view_menu_item = NSMenuItem::new(mtm);
        let view_menu = NSMenu::initWithTitle(
            NSMenu::alloc(mtm),
            &NSString::from_str(&state_ref.translator()._g("View")),
        );
        let full_screen_item = unsafe {
            view_menu.addItemWithTitle_action_keyEquivalent(
                &NSString::from_str(&state_ref.translator()._g("Enter Full Screen")),
                Some(sel!(toggleFullScreen:)),
                ns_string!("f"),
            )
        };
        full_screen_item.setKeyEquivalentModifierMask(
            NSEventModifierFlags::Command | NSEventModifierFlags::Control,
        );
        view_menu_item.setSubmenu(Some(&view_menu));
        this.addItem(&view_menu_item);
        let window_menu_item = NSMenuItem::new(mtm);
        let window_menu = NSMenu::initWithTitle(
            NSMenu::alloc(mtm),
            &NSString::from_str(&state_ref.translator()._g("Window")),
        );
        unsafe {
            window_menu.addItemWithTitle_action_keyEquivalent(
                &NSString::from_str(&state_ref.translator()._g("Minimize")),
                Some(sel!(performMiniaturize:)),
                ns_string!("m"),
            );
            window_menu.addItemWithTitle_action_keyEquivalent(
                &NSString::from_str(&state_ref.translator()._g("Zoom")),
                Some(sel!(performZoom:)),
                ns_string!(""),
            );
        }
        window_menu.addItem(&NSMenuItem::separatorItem(mtm));
        unsafe {
            window_menu.addItemWithTitle_action_keyEquivalent(
                &NSString::from_str(&state_ref.translator()._g("Bring All to Front")),
                Some(sel!(arrangeInFront:)),
                ns_string!(""),
            );
        }
        window_menu_item.setSubmenu(Some(&window_menu));
        NSApplication::sharedApplication(mtm).setWindowsMenu(Some(&window_menu));
        this.addItem(&window_menu_item);
        let help_menu_item = NSMenuItem::new(mtm);
        let help_menu = NSMenu::initWithTitle(
            NSMenu::alloc(mtm),
            &NSString::from_str(&state_ref.translator()._g("Help")),
        );
        unsafe {
            let whats_new_item = help_menu.addItemWithTitle_action_keyEquivalent(
                &NSString::from_str(&state_ref.translator()._g("What's New")),
                Some(sel!(showChangelog:)),
                ns_string!(""),
            );
            let credits_item = help_menu.addItemWithTitle_action_keyEquivalent(
                &NSString::from_str(&state_ref.translator()._g("Credits")),
                Some(sel!(showCredits:)),
                ns_string!(""),
            );
            whats_new_item.setTarget(Some(this.as_super().as_super()));
            credits_item.setTarget(Some(this.as_super().as_super()));
        }
        help_menu_item.setSubmenu(Some(&help_menu));
        NSApplication::sharedApplication(mtm).setHelpMenu(Some(&help_menu));
        this.addItem(&help_menu_item);
        drop(state_ref);
        this
    }
}
