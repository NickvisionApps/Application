use objc2::rc::Retained;
use objc2::runtime::ProtocolObject;
use objc2::{DefinedClass, MainThreadOnly, define_class, msg_send};
use objc2_app_kit::{
    NSBackingStoreType, NSToolbar, NSToolbarDelegate, NSToolbarDisplayMode, NSView, NSWindow,
    NSWindowController, NSWindowDelegate, NSWindowStyleMask, NSWindowToolbarStyle,
};
use objc2_foundation::{MainThreadMarker, NSObjectProtocol, NSPoint, NSRect, NSSize, NSString};
use shared::AppState;
use std::cell::RefCell;
use std::rc::Rc;

#[derive(Debug)]
pub struct MainWindowState {
    state: Rc<RefCell<AppState>>,
}

define_class!(
    #[derive(Debug)]
    #[unsafe(super = NSWindowController)]
    #[thread_kind = MainThreadOnly]
    #[ivars = MainWindowState]
    pub struct MainWindow;

    unsafe impl NSToolbarDelegate for MainWindow {}

    unsafe impl NSObjectProtocol for MainWindow {}

    unsafe impl NSWindowDelegate for MainWindow {
        #[unsafe(method(windowShouldClose:))]
        fn window_show_close(&self, _sender: &NSWindow) -> bool {
            let state_ref = self.ivars().state.borrow();
            state_ref.can_close()
        }
    }
);

impl MainWindowState {
    pub fn new(state: Rc<RefCell<AppState>>) -> Self {
        MainWindowState { state }
    }
}

impl MainWindow {
    pub fn new(mtm: MainThreadMarker, state: Rc<RefCell<AppState>>) -> Retained<Self> {
        let this = Self::alloc(mtm).set_ivars(MainWindowState::new(state));
        let this: Retained<Self> = unsafe { msg_send![super(this), init] };
        let state_ref = this.ivars().state.borrow();
        let geometry = state_ref.configuration().window_geometry();
        let toolbar = NSToolbar::initWithIdentifier(
            NSToolbar::alloc(mtm),
            &NSString::from_str("MainToolbar"),
        );
        toolbar.setDelegate(Some(ProtocolObject::from_ref(&*this)));
        toolbar.setDisplayMode(NSToolbarDisplayMode::IconOnly);
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
                    | NSWindowStyleMask::Resizable
                    | NSWindowStyleMask::UnifiedTitleAndToolbar
                    | NSWindowStyleMask::FullSizeContentView,
                NSBackingStoreType::Buffered,
                false,
            )
        };
        window.setDelegate(Some(ProtocolObject::from_ref(&*this)));
        unsafe { window.setReleasedWhenClosed(false) };
        window.setTitle(&NSString::from_str(
            &state_ref.translator()._g("Application"),
        ));
        window.setTitlebarAppearsTransparent(true);
        window.setContentMinSize(NSSize::new(600.0, 400.0));
        window.setToolbar(Some(&toolbar));
        window.setToolbarStyle(NSWindowToolbarStyle::Unified);
        if let Some(content_view) = window.contentView() {
            let view = NSView::new(mtm);
            content_view.addSubview(&view);
        }
        window.center();
        drop(state_ref);
        this.setWindow(Some(&window));
        this
    }

    pub fn show(&self) {
        unsafe { self.showWindow(None) };
    }

    pub fn close_folder(&self) {}

    pub fn open_folder(&self) {}
}
