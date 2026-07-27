use objc2::rc::Retained;
use objc2::runtime::ProtocolObject;
use objc2::{DefinedClass, MainThreadOnly, define_class, msg_send};
use objc2_app_kit::{NSBackingStoreType, NSLayoutConstraint, NSTabView, NSTabViewItem, NSTabViewType, NSToolbar, NSToolbarDelegate, NSToolbarDisplayMode, NSView, NSWindow, NSWindowController, NSWindowDelegate, NSWindowStyleMask, NSWindowToolbarStyle};
use objc2_foundation::{MainThreadMarker, NSArray, NSObjectProtocol, NSPoint, NSRect, NSSize, NSString};
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

    unsafe impl NSToolbarDelegate for SettingsDialog {}

    unsafe impl NSObjectProtocol for SettingsDialog {}

    unsafe impl NSWindowDelegate for SettingsDialog {
        #[unsafe(method(windowWillClose:))]
        fn window_will_close(&self, _sender: &NSWindow) {
            let state_ref = self.ivars().state.borrow();
            let _ = state_ref.configuration().save();
        }
    }
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
        let toolbar = NSToolbar::initWithIdentifier(
            NSToolbar::alloc(mtm),
            &NSString::from_str("Toolbar"),
        );
        toolbar.setDelegate(Some(ProtocolObject::from_ref(&*this)));
        toolbar.setDisplayMode(NSToolbarDisplayMode::IconAndLabel);
        let window = unsafe {
            NSWindow::initWithContentRect_styleMask_backing_defer(
                NSWindow::alloc(mtm),
                NSRect::new(NSPoint::new(0.0, 0.0), NSSize::new(480.0, 300.0)),
                NSWindowStyleMask::Titled
                    | NSWindowStyleMask::Closable
                    | NSWindowStyleMask::Miniaturizable
                    | NSWindowStyleMask::FullSizeContentView,
                NSBackingStoreType::Buffered,
                false,
            )
        };
        window.setDelegate(Some(ProtocolObject::from_ref(&*this)));
        unsafe { window.setReleasedWhenClosed(false) };
        window.setTitle(&NSString::from_str(&state_ref.translator()._g("General")));
        window.setTitlebarAppearsTransparent(true);
        window.setToolbar(Some(&toolbar));
        window.setToolbarStyle(NSWindowToolbarStyle::Preference);
        if let Some(content_view) = window.contentView() {
            let tab_view = NSTabView::new(mtm);
            tab_view.setTabViewType(NSTabViewType::NoTabsNoBorder);
            NSLayoutConstraint::activateConstraints(&NSArray::from_retained_slice(&[
                tab_view.topAnchor().constraintEqualToAnchor_constant(&content_view.topAnchor(), 100.0),
                tab_view.leadingAnchor().constraintEqualToAnchor(&content_view.leadingAnchor()),
                tab_view.trailingAnchor().constraintEqualToAnchor(&content_view.trailingAnchor()),
                tab_view.bottomAnchor().constraintEqualToAnchor(&content_view.bottomAnchor()),
            ]));
            let general_view = NSView::new(mtm);

            let general_tab = NSTabViewItem::new();
            general_tab.setLabel(&NSString::from_str(&state_ref.translator()._g("General")));
            general_tab.setView(Some(&general_view));
            tab_view.addTabViewItem(&general_tab);
        }
        window.center();
        drop(state_ref);
        this.setWindow(Some(&window));
        this
    }

    pub fn show(&self) {
        unsafe { self.showWindow(None) };
    }
}
