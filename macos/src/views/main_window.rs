use objc2::rc::Retained;
use objc2::runtime::ProtocolObject;
use objc2::{DefinedClass, MainThreadOnly, define_class, msg_send};
use objc2_app_kit::{
    NSApplication, NSBackingStoreType, NSWindow, NSWindowDelegate, NSWindowStyleMask,
};
use objc2_foundation::{
    MainThreadMarker, NSNotification, NSObject, NSObjectProtocol, NSPoint, NSRect, NSSize, NSString,
};
use shared::AppState;
use std::cell::{OnceCell, RefCell};
use std::rc::Rc;

#[derive(Debug)]
pub struct MainWindowState {
    state: Rc<RefCell<AppState>>,
    window: OnceCell<Retained<NSWindow>>,
}

define_class!(
    #[derive(Debug)]
    #[unsafe(super = NSObject)]
    #[thread_kind = MainThreadOnly]
    #[ivars = MainWindowState]
    pub struct MainWindow;

    unsafe impl NSObjectProtocol for MainWindow {}

    unsafe impl NSWindowDelegate for MainWindow {
        #[unsafe(method(windowWillClose:))]
        fn window_will_close(&self, _notification: &NSNotification) {
            NSApplication::sharedApplication(self.mtm()).terminate(None)
        }
    }
);

impl MainWindowState {
    pub fn new(state: Rc<RefCell<AppState>>) -> Self {
        MainWindowState {
            state,
            window: OnceCell::<Retained<NSWindow>>::default(),
        }
    }
}

impl MainWindow {
    pub fn new(mtm: MainThreadMarker, state: Rc<RefCell<AppState>>) -> Retained<Self> {
        let this = Self::alloc(mtm).set_ivars(MainWindowState::new(state));
        let this: Retained<Self> = unsafe { msg_send![super(this), init] };
        let state_ref = this.ivars().state.borrow();
        let geometry = state_ref.configuration().window_geometry();
        let window = unsafe {
            NSWindow::initWithContentRect_styleMask_backing_defer(
                NSWindow::alloc(mtm),
                NSRect::new(
                    NSPoint::new(geometry.x() as f64, geometry.y() as f64),
                    NSSize::new(geometry.width() as f64, geometry.height() as f64),
                ),
                NSWindowStyleMask::Titled
                    | NSWindowStyleMask::Closable
                    | NSWindowStyleMask::Miniaturizable
                    | NSWindowStyleMask::Resizable,
                NSBackingStoreType::Buffered,
                false,
            )
        };
        window.setDelegate(Some(ProtocolObject::from_ref(&*this)));
        unsafe { window.setReleasedWhenClosed(false) };
        window.setTitle(&NSString::from_str(
            &this.ivars().state.borrow().translator()._g("Application"),
        ));
        window.setContentMinSize(NSSize::new(
            geometry.width() as f64,
            geometry.height() as f64,
        ));
        window.center();
        this.ivars().window.set(window).unwrap();
        drop(state_ref);
        this
    }

    pub fn ns_window(&self) -> &NSWindow {
        self.ivars().window.get().expect("main window must exist")
    }
}
