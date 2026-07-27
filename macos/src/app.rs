use crate::views::MainWindow;
use objc2::rc::Retained;
use objc2::runtime::AnyObject;
use objc2::{ClassType, DefinedClass, MainThreadOnly, define_class, msg_send, sel};
use objc2_app_kit::{
    NSAppearance, NSAppearanceNameAqua, NSAppearanceNameDarkAqua, NSApplication,
    NSApplicationActivationPolicy, NSApplicationDelegate, NSEventModifierFlags, NSImage, NSMenu,
    NSMenuItem,
};
use objc2_foundation::{
    MainThreadMarker, NSNotification, NSObject, NSObjectProtocol, NSString, ns_string,
};
use shared::{AppState, ApplicationTheme};
use std::cell::{OnceCell, RefCell};
use std::rc::Rc;

#[derive(Debug, Default)]
pub struct DelegateState {
    state: Rc<RefCell<AppState>>,
    window: OnceCell<Retained<MainWindow>>,
}

define_class!(
    #[derive(Debug)]
    #[unsafe(super = NSObject)]
    #[thread_kind = MainThreadOnly]
    #[ivars = DelegateState]
    pub struct Delegate;

    impl Delegate {
        #[unsafe(method(checkForUpdates:))]
        fn check_for_updates(&self, _sender: Option<&AnyObject>) {
        }

        #[unsafe(method(showSettings:))]
        fn show_settings(&self, _sender: Option<&AnyObject>) {
        }
    }

    unsafe impl NSObjectProtocol for Delegate {}

    unsafe impl NSApplicationDelegate for Delegate {
        #[unsafe(method(applicationDidFinishLaunching:))]
        fn did_finish_launching(&self, notification: &NSNotification) {
            let app = notification
                .object()
                .unwrap()
                .downcast::<NSApplication>()
                .unwrap();
            self.ivars()
                .window
                .set(MainWindow::new(self.mtm(), Rc::clone(&self.ivars().state)))
                .unwrap();
            self.ivars().window.get().unwrap().show();
            app.setActivationPolicy(NSApplicationActivationPolicy::Regular);
            app.activate();
        }

        #[unsafe(method(applicationShouldTerminateAfterLastWindowClosed:))]
        fn should_terminate_after_last_window_closed(&self, _sender: &NSApplication) -> bool {
            false
        }

        #[unsafe(method(applicationShouldHandleReopen:hasVisibleWindows:))]
        fn should_handle_reopen(&self, _sender: &NSApplication, has_visible_windows: bool) -> bool {
            if !has_visible_windows {
                if let Some(window) = self.ivars().window.get() {
                    window.show();
                }
                NSApplication::sharedApplication(self.mtm()).activate();
            }
            true
        }
    }
);

impl Delegate {
    pub fn new(mtm: MainThreadMarker) -> Retained<Self> {
        let this = Self::alloc(mtm).set_ivars(DelegateState::default());
        let this: Retained<Self> = unsafe { msg_send![super(this), init] };
        let state_ref = this.ivars().state.borrow();
        let app = NSApplication::sharedApplication(mtm);
        let theme = match state_ref.configuration().theme() {
            ApplicationTheme::Light => unsafe {
                NSAppearance::appearanceNamed(NSAppearanceNameAqua)
            },
            ApplicationTheme::Dark => unsafe {
                NSAppearance::appearanceNamed(NSAppearanceNameDarkAqua)
            },
            ApplicationTheme::System => None,
        };
        app.setAppearance(theme.as_deref());
        let main_menu = NSMenu::initWithTitle(NSMenu::alloc(mtm), ns_string!("MainMenu"));
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
            app_menu.addItemWithTitle_action_keyEquivalent(
                &NSString::from_str(&state_ref.translator()._g("Check for Updates")),
                Some(sel!(checkForUpdates:)),
                ns_string!(""),
            );
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
        app.setServicesMenu(Some(&services_menu));
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
        main_menu.addItem(&app_menu_item);
        let file_menu_item = NSMenuItem::new(mtm);
        let file_menu = NSMenu::initWithTitle(
            NSMenu::alloc(mtm),
            &NSString::from_str(&state_ref.translator()._g("File")),
        );
        unsafe {
            file_menu.addItemWithTitle_action_keyEquivalent(
                &NSString::from_str(&state_ref.translator()._g("Close")),
                Some(sel!(performClose:)),
                ns_string!("w"),
            );
        }
        file_menu_item.setSubmenu(Some(&file_menu));
        main_menu.addItem(&file_menu_item);
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
        main_menu.addItem(&edit_menu_item);
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
        main_menu.addItem(&view_menu_item);
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
        app.setWindowsMenu(Some(&window_menu));
        main_menu.addItem(&window_menu_item);
        let help_menu_item = NSMenuItem::new(mtm);
        let help_menu = NSMenu::initWithTitle(
            NSMenu::alloc(mtm),
            &NSString::from_str(&state_ref.translator()._g("Help")),
        );
        unsafe {
            help_menu.addItemWithTitle_action_keyEquivalent(
                &NSString::from_str(&state_ref.translator()._g("Application Help")),
                Some(sel!(showHelp:)),
                ns_string!("?"),
            );
        }
        help_menu_item.setSubmenu(Some(&help_menu));
        app.setHelpMenu(Some(&help_menu));
        main_menu.addItem(&help_menu_item);
        app.setMainMenu(Some(&main_menu));
        drop(state_ref);
        this
    }
}
