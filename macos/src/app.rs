use crate::views::MainWindow;
use objc2::rc::Retained;
use objc2::{DefinedClass, MainThreadOnly, define_class, msg_send, sel};
use objc2_app_kit::{
    NSApplication, NSApplicationActivationPolicy, NSApplicationDelegate, NSEventModifierFlags,
    NSMenu, NSMenuItem,
};
use objc2_foundation::{
    MainThreadMarker, NSNotification, NSObject, NSObjectProtocol, NSString, ns_string,
};
use shared::AppState;
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

    unsafe impl NSObjectProtocol for Delegate {}

    unsafe impl NSApplicationDelegate for Delegate {
        #[unsafe(method(applicationDidFinishLaunching:))]
        fn did_finish_launching(&self, notification: &NSNotification) {
            let app = notification
                .object()
                .unwrap()
                .downcast::<NSApplication>()
                .unwrap();
            let state_ref = self.ivars().state.borrow();
            let main_menu =
                NSMenu::initWithTitle(NSMenu::alloc(self.mtm()), ns_string!("MainMenu"));
            let app_menu_item = NSMenuItem::new(self.mtm());
            main_menu.addItem(&app_menu_item);
            let app_menu = NSMenu::initWithTitle(
                NSMenu::alloc(self.mtm()),
                &NSString::from_str(&state_ref.translator()._g("Application")),
            );
            unsafe {
                app_menu.addItemWithTitle_action_keyEquivalent(
                    &NSString::from_str(&state_ref.translator()._g("About Application")),
                    Some(sel!(orderFrontStandardAboutPanel:)),
                    ns_string!(""),
                );
            }
            app_menu.addItem(&NSMenuItem::separatorItem(self.mtm()));
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
            app_menu.addItem(&NSMenuItem::separatorItem(self.mtm()));
            unsafe {
                app_menu.addItemWithTitle_action_keyEquivalent(
                    &NSString::from_str(&state_ref.translator()._g("Quit Application")),
                    Some(sel!(terminate:)),
                    ns_string!("q"),
                );
            }
            app_menu_item.setSubmenu(Some(&app_menu));
            let edit_menu_item = NSMenuItem::new(self.mtm());
            main_menu.addItem(&edit_menu_item);
            let edit_menu = NSMenu::initWithTitle(
                NSMenu::alloc(self.mtm()),
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
            edit_menu.addItem(&NSMenuItem::separatorItem(self.mtm()));
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
            app.setMainMenu(Some(&main_menu));
            drop(state_ref);
            self.ivars()
                .window
                .set(MainWindow::new(self.mtm(), Rc::clone(&self.ivars().state)))
                .unwrap();
            self.ivars().window.get().unwrap().show();
            app.setActivationPolicy(NSApplicationActivationPolicy::Regular);
            app.activate();
        }
    }
);

impl Delegate {
    pub fn new(mtm: MainThreadMarker) -> Retained<Self> {
        let this = Self::alloc(mtm).set_ivars(DelegateState::default());
        unsafe { msg_send![super(this), init] }
    }
}
