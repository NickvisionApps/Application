use crate::helpers::EasyToolbarItem;
use objc2::rc::Retained;
use objc2::runtime::{AnyObject, ProtocolObject};
use objc2::{ClassType, DefinedClass, MainThreadOnly, Message, define_class, msg_send, sel};
use objc2_app_kit::{
    NSAlert, NSAlertFirstButtonReturn, NSBackingStoreType, NSModalResponseOK, NSOpenPanel,
    NSToolbar, NSToolbarDelegate, NSToolbarDisplayMode, NSToolbarFlexibleSpaceItemIdentifier,
    NSToolbarItem, NSToolbarItemIdentifier, NSToolbarSpaceItemIdentifier, NSView, NSWindow,
    NSWindowController, NSWindowDelegate, NSWindowStyleMask, NSWindowToolbarStyle,
};
use objc2_foundation::{
    MainThreadMarker, NSArray, NSNotification, NSObjectProtocol, NSPoint, NSRect, NSSize, NSString,
    ns_string,
};
use shared::{config::WindowGeometry, controller::AppController, info, translation};
use std::{cell::RefCell, rc::Rc};

#[derive(Debug)]
pub struct MainWindowState {
    controller: Rc<RefCell<AppController>>,
}

define_class!(
    #[derive(Debug)]
    #[unsafe(super = NSWindowController)]
    #[thread_kind = MainThreadOnly]
    #[ivars = MainWindowState]
    pub struct MainWindow;

    impl MainWindow {
        #[unsafe(method(closeFolderClicked:))]
        fn close_folder_clicked(&self, sender: Option<&AnyObject>) {
            self.close_folder();
        }

        #[unsafe(method(openFolderClicked:))]
        fn open_folder_clicked(&self, sender: Option<&AnyObject>) {
            self.open_folder();
        }
    }

    unsafe impl NSToolbarDelegate for MainWindow {
        #[unsafe(method_id(toolbarDefaultItemIdentifiers:))]
        fn toolbar_default_item_identifiers(
            &self,
            _toolbar: &NSToolbar,
        ) -> Retained<NSArray<NSToolbarItemIdentifier>> {
            NSArray::from_retained_slice(&[
                NSToolbarItemIdentifier::from_str("OpenFolder"),
                NSToolbarItemIdentifier::from_str("CloseFolder"),
            ])
        }

        #[unsafe(method_id(toolbarAllowedItemIdentifiers:))]
        fn toolbar_allowed_item_identifiers(
            &self,
            _toolbar: &NSToolbar,
        ) -> Retained<NSArray<NSToolbarItemIdentifier>> {
            NSArray::from_retained_slice(&[
                unsafe { NSToolbarSpaceItemIdentifier.retain() },
                unsafe { NSToolbarFlexibleSpaceItemIdentifier.retain() },
                NSToolbarItemIdentifier::from_str("OpenFolder"),
                NSToolbarItemIdentifier::from_str("CloseFolder"),
            ])
        }

        #[unsafe(method_id(toolbar:itemForItemIdentifier:willBeInsertedIntoToolbar:))]
        fn toolbar_item_for_item_identifier(
            &self,
            toolbar: &NSToolbar,
            item_identifier: &NSToolbarItemIdentifier,
            will_be_inserted: bool,
        ) -> Option<Retained<NSToolbarItem>> {
            if item_identifier == ns_string!("CloseFolder") {
                NSToolbarItem::init_easy(
                    self.mtm(),
                    item_identifier,
                    translation::_g("Close Folder"),
                    "folder.badge.minus",
                    Some(self.as_super().as_super()),
                    sel!(closeFolderClicked:)
                )
            } else if item_identifier == ns_string!("OpenFolder") {
                NSToolbarItem::init_easy(
                    self.mtm(),
                    item_identifier,
                    translation::_g("Open Folder"),
                    "folder.badge.plus",
                    Some(self.as_super().as_super()),
                    sel!(openFolderClicked:)
                )
            } else {
                None
            }
        }
    }

    unsafe impl NSObjectProtocol for MainWindow {}

    unsafe impl NSWindowDelegate for MainWindow {
        #[unsafe(method(windowShouldClose:))]
        fn window_should_close(&self, _sender: &NSWindow) -> bool {
            self.ivars().controller.borrow().can_close()
        }

        #[unsafe(method(windowWillClose:))]
        fn window_will_close(&self, _notification: &NSNotification) {
            let mut controller = self.ivars().controller.borrow_mut();
            let window = self.window().unwrap();
            if window.isZoomed() {
                controller
                    .set_window_geometry(WindowGeometry::builder().is_maximized(true).build());
            } else {
                let frame = window.frame();
                controller.set_window_geometry(
                    WindowGeometry::builder()
                        .x(frame.origin.x as i64)
                        .y(frame.origin.y as i64)
                        .width(frame.size.width as u64)
                        .height(frame.size.height as u64)
                        .build(),
                );
            }
            controller.save().unwrap();
        }
    }
);

impl MainWindowState {
    pub fn new(controller: Rc<RefCell<AppController>>) -> Self {
        MainWindowState { controller }
    }
}

impl MainWindow {
    pub fn new(mtm: MainThreadMarker, controller: Rc<RefCell<AppController>>) -> Retained<Self> {
        let this = Self::alloc(mtm).set_ivars(MainWindowState::new(controller));
        let this: Retained<Self> = unsafe { msg_send![super(this), init] };
        let controller = this.ivars().controller.borrow();
        let geometry = controller.window_geometry();
        let toolbar = NSToolbar::initWithIdentifier(
            NSToolbar::alloc(mtm),
            &NSString::from_str("MainToolbar"),
        );
        toolbar.setDelegate(Some(ProtocolObject::from_ref(&*this)));
        toolbar.setDisplayMode(NSToolbarDisplayMode::IconOnly);
        toolbar.setAutosavesConfiguration(true);
        toolbar.setAllowsUserCustomization(true);
        toolbar.setAllowsDisplayModeCustomization(false);
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
        window.setTitle(&NSString::from_str(&translation::_g("Application")));
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
        drop(controller);
        this.setWindow(Some(&window));
        this
    }

    pub fn check_for_updates(&self) {
        let controller = self.ivars().controller.borrow().clone();
        tokio::spawn(async move {
            let version = controller.check_for_updates().await;
            dispatch2::run_on_main(move |mtm| {
                let alert = NSAlert::new(mtm);
                if let Some(ref version) = version {
                    alert.setMessageText(&NSString::from_str(&translation::_g("Update Available")));
                    alert.setInformativeText(&NSString::from_str(&translation::_f(
                        "A new update for {0} is available: {1}",
                        &[info::APP_ENGLISH_SHORT_NAME, &version.to_string()],
                    )));
                    alert.addButtonWithTitle(&NSString::from_str(&translation::_g("Update")));
                    alert.addButtonWithTitle(&NSString::from_str(&translation::_g("OK")));
                } else {
                    alert.setMessageText(&NSString::from_str(&translation::_g(
                        "No Update Available",
                    )));
                    alert.setInformativeText(&NSString::from_str(&translation::_f(
                        "You are running the latest version of {0}.",
                        &[info::APP_ENGLISH_SHORT_NAME],
                    )));
                }
                if alert.runModal() == NSAlertFirstButtonReturn && version.is_some() {
                    tokio::spawn(async move {
                        if let Err(error) = controller
                            .install_update(move |_downloaded, _total| {
                                dispatch2::run_on_main(move |_mtm| {
                                    //TODO: Update UI with progress
                                });
                            })
                            .await
                        {
                            let error_msg = error.to_string();
                            dispatch2::run_on_main(move |mtm| {
                                let alert = NSAlert::new(mtm);
                                alert
                                    .setMessageText(&NSString::from_str(&translation::_g("Error")));
                                alert.setInformativeText(&NSString::from_str(&translation::_f(
                                    "Unable to install the update: {0}",
                                    &[error_msg],
                                )));
                                alert.runModal();
                            });
                        }
                    });
                }
            });
        });
    }

    pub fn close_folder(&self) {
        let mut controller = self.ivars().controller.borrow_mut();
        controller.folder_browser_mut().close();
        //TODO: Update UI
    }

    pub fn open_folder(&self) {
        let open_panel = NSOpenPanel::new(self.mtm());
        open_panel.setCanChooseFiles(false);
        open_panel.setCanChooseDirectories(true);
        open_panel.setAllowsMultipleSelection(false);
        if open_panel.runModal() == NSModalResponseOK {
            let mut controller = self.ivars().controller.borrow_mut();
            if let Err(error) = controller.folder_browser_mut().open(
                open_panel
                    .URLs()
                    .firstObject()
                    .unwrap()
                    .path()
                    .unwrap()
                    .to_string(),
            ) {
                let alert = NSAlert::new(self.mtm());
                alert.setMessageText(&NSString::from_str(&translation::_g("Error")));
                alert.setInformativeText(&NSString::from_str(&translation::_f(
                    "Unable to open folder: {0}",
                    &[error.to_string()],
                )));
                alert.runModal();
            } else {
                //TODO: Update UI
            }
        }
    }

    pub fn show(&self) {
        unsafe { self.showWindow(None) };
    }
}
