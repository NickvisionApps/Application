use crate::views::MainWindow;
use objc2::rc::Retained;
use objc2::{DefinedClass, MainThreadOnly, define_class, msg_send};
use objc2_app_kit::{NSApplication, NSApplicationActivationPolicy, NSApplicationDelegate};
use objc2_foundation::{MainThreadMarker, NSNotification, NSObject, NSObjectProtocol};
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
