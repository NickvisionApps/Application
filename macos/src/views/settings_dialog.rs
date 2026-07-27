use objc2::rc::Retained;
use objc2::{DefinedClass, MainThreadOnly, define_class, msg_send};
use objc2_app_kit::{NSBackingStoreType, NSWindow, NSWindowController, NSWindowStyleMask};
use objc2_foundation::{MainThreadMarker, NSObjectProtocol, NSPoint, NSRect, NSSize, NSString};
use shared::AppState;
use std::cell::RefCell;
use std::rc::Rc;

#[derive(Debug)]
pub struct SettingsDialogState {
    state: Rc<RefCell<AppState>>,
}

define_class!(
    #[derive(Debug)]
    #[unsafe(super = NSWindowController)]
    #[thread_kind = MainThreadOnly]
    #[ivars = SettingsDialogState]
    pub struct SettingsDialog;

    unsafe impl NSObjectProtocol for SettingsDialog {}
);

impl SettingsDialogState {
    pub fn new(state: Rc<RefCell<AppState>>) -> Self {
        SettingsDialogState { state }
    }
}

impl SettingsDialog {
    pub fn new(mtm: MainThreadMarker, state: Rc<RefCell<AppState>>) -> Retained<Self> {
        let this = Self::alloc(mtm).set_ivars(SettingsDialogState::new(state));
        let this: Retained<Self> = unsafe { msg_send![super(this), init] };
        let state_ref = this.ivars().state.borrow();
        let window = unsafe {
            NSWindow::initWithContentRect_styleMask_backing_defer(
                NSWindow::alloc(mtm),
                NSRect::new(NSPoint::new(0.0, 0.0), NSSize::new(480.0, 300.0)),
                NSWindowStyleMask::Titled | NSWindowStyleMask::Closable,
                NSBackingStoreType::Buffered,
                false,
            )
        };
        this.setWindow(Some(&window));
        unsafe { window.setReleasedWhenClosed(false) };
        window.setTitle(&NSString::from_str(&state_ref.translator()._g("Settings")));
        window.center();
        drop(state_ref);
        this
    }

    pub fn show(&self) {
        unsafe { self.showWindow(None) };
    }
}
