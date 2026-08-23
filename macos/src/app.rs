use crate::controls::MainMenu;
use crate::views::MainWindow;
use objc2::rc::Retained;
use objc2::{DefinedClass, MainThreadOnly, define_class, msg_send};
use objc2_app_kit::{
    NSAppearance, NSAppearanceNameAqua, NSAppearanceNameDarkAqua, NSApplication,
    NSApplicationActivationPolicy, NSApplicationDelegate, NSApplicationTerminateReply,
};
use objc2_foundation::{MainThreadMarker, NSNotification, NSObject, NSObjectProtocol};
use shared::{config::ApplicationTheme, controller::AppController};
use std::cell::{OnceCell, RefCell};
use std::rc::Rc;

#[derive(Debug, Default)]
pub struct DelegateState {
    controller: Rc<RefCell<AppController>>,
    main_menu: OnceCell<Retained<MainMenu>>,
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
            let theme = match self.ivars().controller.borrow().theme() {
                ApplicationTheme::Light => unsafe {
                    NSAppearance::appearanceNamed(NSAppearanceNameAqua)
                },
                ApplicationTheme::Dark => unsafe {
                    NSAppearance::appearanceNamed(NSAppearanceNameDarkAqua)
                },
                ApplicationTheme::System => None,
            };
            let window = MainWindow::new(self.mtm(), self.ivars().controller.clone());
            self.ivars()
                .main_menu
                .set(MainMenu::new(
                    self.mtm(),
                    self.ivars().controller.clone(),
                    window.clone(),
                ))
                .unwrap();
            self.ivars().window.set(window).unwrap();
            app.setActivationPolicy(NSApplicationActivationPolicy::Regular);
            app.setAppearance(theme.as_deref());
            app.setMainMenu(Some(self.ivars().main_menu.get().unwrap()));
            self.ivars().window.get().unwrap().show();
            app.activate();
        }

        #[unsafe(method(applicationShouldTerminate:))]
        fn should_terminate(&self, _sender: &NSApplication) -> NSApplicationTerminateReply {
            if self.ivars().controller.borrow().can_close() {
                if let Some(window) = self.ivars().window.get() {
                    window.close();
                }
                NSApplicationTerminateReply::TerminateNow
            } else {
                NSApplicationTerminateReply::TerminateCancel
            }
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
        unsafe { msg_send![super(this), init] }
    }
}
