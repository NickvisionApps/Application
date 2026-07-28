use objc2::rc::Retained;
use objc2::runtime::ProtocolObject;
use objc2::{DefinedClass, MainThreadOnly, define_class, msg_send};
use objc2_app_kit::{
    NSAlert, NSAlertFirstButtonReturn, NSBackingStoreType, NSModalResponseOK, NSOpenPanel,
    NSToolbar, NSToolbarDelegate, NSToolbarDisplayMode, NSView, NSWindow, NSWindowController,
    NSWindowDelegate, NSWindowStyleMask, NSWindowToolbarStyle,
};
use objc2_foundation::{
    MainThreadMarker, NSNotification, NSObjectProtocol, NSPoint, NSRect, NSSize, NSString,
};
use shared::{APP_ENGLISH_SHORT_NAME, AppState, WindowGeometry};
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
        fn should_close(&self, _sender: &NSWindow) -> bool {
            self.ivars().state.borrow().can_close()
        }

        #[unsafe(method(windowWillClose:))]
        fn will_close(&self, _notification: &NSNotification) {
            let mut state_ref = self.ivars().state.borrow_mut();
            let configuration = state_ref.configuration_mut();
            let window = self.window().unwrap();
            if window.isZoomed() {
                configuration
                    .set_window_geometry(WindowGeometry::builder().is_maximized(true).build());
            } else {
                let frame = window.frame();
                configuration.set_window_geometry(
                    WindowGeometry::builder()
                        .x(frame.origin.x as i64)
                        .y(frame.origin.y as i64)
                        .width(frame.size.width as u64)
                        .height(frame.size.height as u64)
                        .build(),
                );
            }
            configuration.save().unwrap();
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
        if geometry.is_maximized() {
            window.setIsZoomed(true);
        }
        drop(state_ref);
        this.setWindow(Some(&window));
        this
    }

    pub fn check_for_updates(&self) {
        let state_ref = self.ivars().state.borrow();
        let updater = state_ref.updater();
        let translator = state_ref.translator();
        tokio::spawn(async move {
            let version = updater.check_for_updates().await;
            dispatch2::run_on_main(move |mtm| {
                let alert = NSAlert::new(mtm);
                if let Some(ref version) = version {
                    alert.setMessageText(&NSString::from_str(&translator._g("Update Available")));
                    alert.setInformativeText(&NSString::from_str(&translator._f(
                        "A new update for {0} is available: {1}",
                        &[APP_ENGLISH_SHORT_NAME, &version.to_string()],
                    )));
                    alert.addButtonWithTitle(&NSString::from_str(&translator._g("Update")));
                    alert.addButtonWithTitle(&NSString::from_str(&translator._g("OK")));
                } else {
                    alert
                        .setMessageText(&NSString::from_str(&translator._g("No Update Available")));
                    alert.setInformativeText(&NSString::from_str(&translator._f(
                        "You are running the latest version of {0}.",
                        &[APP_ENGLISH_SHORT_NAME],
                    )));
                }
                if alert.runModal() == NSAlertFirstButtonReturn && version.is_some() {
                    tokio::spawn(async move {
                        updater
                            .install_update(move |downloaded, total| {
                                dispatch2::run_on_main(move |mtm| {
                                    //TODO: Update UI
                                });
                            })
                            .await;
                    });
                }
            });
        });
    }

    pub fn close_folder(&self) {
        let mut state_ref = self.ivars().state.borrow_mut();
        state_ref.folder_browser_mut().close();
        //TODO: Update UI
    }

    pub fn open_folder(&self) {
        let open_panel = NSOpenPanel::new(self.mtm());
        open_panel.setCanChooseFiles(false);
        open_panel.setCanChooseDirectories(true);
        open_panel.setAllowsMultipleSelection(false);
        if open_panel.runModal() == NSModalResponseOK {
            let mut state_ref = self.ivars().state.borrow_mut();
            if state_ref
                .folder_browser_mut()
                .open(
                    open_panel
                        .URLs()
                        .firstObject()
                        .unwrap()
                        .path()
                        .unwrap()
                        .to_string(),
                )
                .is_ok()
            {
                //TODO: Update UI
            }
        }
    }

    pub fn show(&self) {
        unsafe { self.showWindow(None) };
    }
}
