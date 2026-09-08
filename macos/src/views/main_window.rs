use crate::controls::{FolderPage, HomePage, UpdateProgressPage};
use crate::helpers::{EasyLayout, EasyToolbarItem};
use dispatch2::{MainThreadBound, run_on_main};
use objc2::rc::Retained;
use objc2::runtime::{AnyObject, ProtocolObject};
use objc2::{ClassType, DefinedClass, MainThreadOnly, Message, define_class, msg_send, sel};
use objc2_app_kit::{
    NSAlert, NSAlertFirstButtonReturn, NSBackingStoreType, NSItemBadge, NSModalResponseOK,
    NSOpenPanel, NSPopover, NSPopoverBehavior, NSTabView, NSTabViewItem, NSTabViewType, NSToolbar,
    NSToolbarDelegate, NSToolbarDisplayMode, NSToolbarFlexibleSpaceItemIdentifier,
    NSToolbarIdentifier, NSToolbarItem, NSToolbarItemIdentifier, NSToolbarSpaceItemIdentifier,
    NSWindow, NSWindowController, NSWindowDelegate, NSWindowStyleMask, NSWindowToolbarStyle,
};
use objc2_foundation::{
    MainThreadMarker, NSArray, NSNotification, NSObjectProtocol, NSPoint, NSRect, NSSize, NSString,
    ns_string,
};
use shared::{config::WindowGeometry, controller::AppController, info, translation};
use std::cell::OnceCell;
use std::ops::ControlFlow;
use std::sync::Arc;
use std::sync::atomic::{AtomicBool, Ordering};
use std::{cell::RefCell, rc::Rc};

#[derive(Debug)]
pub struct MainWindowControls {
    update_progress_popover: Retained<NSPopover>,
    update_progress_page: Retained<UpdateProgressPage>,
    toolbar: Retained<NSToolbar>,
    tab_view: Retained<NSTabView>,
    home_page: Retained<HomePage>,
    folder_page: Retained<FolderPage>,
}

#[derive(Debug)]
pub struct MainWindowState {
    controller: Rc<RefCell<AppController>>,
    controls: OnceCell<MainWindowControls>,
    cancel_update: Arc<AtomicBool>,
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

        #[unsafe(method(updateProgressClicked:))]
        fn update_progress_clicked(&self, sender: Option<&AnyObject>) {
            let controls = self.ivars().controls.get().unwrap();
            if controls.update_progress_popover.isShown() {
                unsafe { controls.update_progress_popover.performClose(sender) };
            } else if let Some(sender) = sender {
                controls
                    .update_progress_popover
                    .showRelativeToToolbarItem(sender.downcast_ref::<NSToolbarItem>().unwrap());
            }
        }

        #[unsafe(method(cancelUpdateClicked:))]
        fn cancel_update_clicked(&self, _sender: Option<&AnyObject>) {
            self.ivars().cancel_update.store(true, Ordering::Relaxed);
            self.ivars()
                .controls
                .get()
                .unwrap()
                .update_progress_page
                .reset();
        }
    }

    unsafe impl NSToolbarDelegate for MainWindow {
        #[unsafe(method_id(toolbarDefaultItemIdentifiers:))]
        fn toolbar_default_item_identifiers(
            &self,
            _toolbar: &NSToolbar,
        ) -> Retained<NSArray<NSToolbarItemIdentifier>> {
            NSArray::from_retained_slice(&[
                unsafe { NSToolbarSpaceItemIdentifier.retain() },
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
                NSToolbarItemIdentifier::from_str("CloseFolder"),
                NSToolbarItemIdentifier::from_str("OpenFolder"),
                NSToolbarItemIdentifier::from_str("UpdateProgress"),
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
                    translation::_g("Close Folder (⇧⌘W)"),
                    Some(self.as_super().as_super()),
                    sel!(closeFolderClicked:)
                )
            } else if item_identifier == ns_string!("OpenFolder") {
                NSToolbarItem::init_easy(
                    self.mtm(),
                    item_identifier,
                    translation::_g("Open Folder"),
                    "folder.badge.plus",
                    translation::_g("Open Folder (⌘O)"),
                    Some(self.as_super().as_super()),
                    sel!(openFolderClicked:)
                )
            } else if item_identifier == ns_string!("UpdateProgress") {
                NSToolbarItem::init_easy(
                    self.mtm(),
                    item_identifier,
                    translation::_g("Update Progress"),
                    "arrow.down.circle",
                    "",
                    Some(self.as_super().as_super()),
                    sel!(updateProgressClicked:)
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
        MainWindowState {
            controller,
            controls: OnceCell::new(),
            cancel_update: Arc::new(AtomicBool::new(false)),
        }
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
        let update_progress_popover = NSPopover::new(mtm);
        update_progress_popover.setBehavior(NSPopoverBehavior::Transient);
        update_progress_popover.setAnimates(true);
        let update_progress_page = UpdateProgressPage::new(
            mtm,
            Some(this.as_super().as_super()),
            sel!(cancelUpdateClicked:),
        );
        update_progress_popover.setContentViewController(Some(&update_progress_page));
        if let Some(content_view) = window.contentView() {
            let tab_view = NSTabView::new(mtm);
            tab_view.setTabViewType(NSTabViewType::NoTabsNoBorder);
            let home_page = HomePage::new(
                mtm,
                Some(this.as_super().as_super()),
                sel!(openFolderClicked:),
            );
            home_page.set_greeting(&NSString::from_str(&controller.greeting()));
            let folder_page = FolderPage::new(mtm);
            let home_tab = NSTabViewItem::new();
            home_tab.setView(Some(&home_page.view()));
            tab_view.addTabViewItem(&home_tab);
            let folder_tab = NSTabViewItem::new();
            folder_tab.setView(Some(&folder_page.view()));
            tab_view.addTabViewItem(&folder_tab);
            tab_view.selectTabViewItemAtIndex(HomePage::page_index());
            content_view.addSubview(&tab_view);
            tab_view.constrain_fill(&content_view);
            this.ivars()
                .controls
                .set(MainWindowControls {
                    update_progress_popover,
                    update_progress_page,
                    toolbar,
                    tab_view,
                    home_page,
                    folder_page,
                })
                .unwrap();
        }
        if geometry.is_maximized() {
            window.setIsZoomed(true);
        }
        drop(controller);
        this.setWindow(Some(&window));
        this
    }

    pub fn check_for_updates(&self, notify_if_up_to_date: bool) {
        let controller = self.ivars().controller.borrow().clone();
        let window_bound = MainThreadBound::new(self.retain(), self.mtm());
        std::thread::spawn(move || {
            let version = controller.check_for_updates();
            run_on_main(move |mtm| {
                let window = window_bound.get(mtm);
                if let Some(version) = version {
                    let alert = NSAlert::new(mtm);
                    alert.setMessageText(&NSString::from_str(&translation::_g("Update Available")));
                    alert.setInformativeText(&NSString::from_str(&translation::_f(
                        "A new update for {0} is available: {1}",
                        &[info::APP_ENGLISH_SHORT_NAME, &version.to_string()],
                    )));
                    alert.addButtonWithTitle(&NSString::from_str(&translation::_g("Update")));
                    alert.addButtonWithTitle(&NSString::from_str(&translation::_g("OK")));
                    if alert.runModal() != NSAlertFirstButtonReturn {
                        return;
                    }
                    let controls = window.ivars().controls.get().unwrap();
                    if !controls
                        .toolbar
                        .items()
                        .iter()
                        .any(|item| &*item.itemIdentifier() == ns_string!("UpdateProgress"))
                    {
                        controls.toolbar.insertItemWithItemIdentifier_atIndex(
                            &NSToolbarIdentifier::from_str("UpdateProgress"),
                            0,
                        );
                    }
                    if let Some(item) = controls
                        .toolbar
                        .items()
                        .iter()
                        .find(|item| &*item.itemIdentifier() == ns_string!("UpdateProgress"))
                    {
                        item.setBadge(Some(&NSItemBadge::indicatorBadge()));
                    }
                    controls.update_progress_page.set_progress(0, 0);
                    window.ivars().cancel_update.store(false, Ordering::Relaxed);
                    let cancel_update = window.ivars().cancel_update.clone();
                    let controller = window.ivars().controller.borrow().clone();
                    let window_bound = MainThreadBound::new(window.retain(), mtm);
                    std::thread::spawn(move || {
                        let result = controller.install_update(|downloaded, total| {
                            if cancel_update.load(Ordering::Relaxed) {
                                return ControlFlow::Break(());
                            }
                            run_on_main(|mtm| {
                                window_bound
                                    .get(mtm)
                                    .ivars()
                                    .controls
                                    .get()
                                    .unwrap()
                                    .update_progress_page
                                    .set_progress(downloaded, total)
                            });
                            ControlFlow::Continue(())
                        });
                        let cancelled = cancel_update.load(Ordering::Relaxed);
                        let error = result.err().map(|error| error.to_string());
                        run_on_main(move |mtm| {
                            let window = window_bound.get(mtm);
                            let controls = window.ivars().controls.get().unwrap();
                            if let Some(index) = controls.toolbar.items().iter().position(|item| {
                                &*item.itemIdentifier() == ns_string!("UpdateProgress")
                            }) {
                                controls.toolbar.removeItemAtIndex(index as isize);
                            }
                            unsafe { controls.update_progress_popover.performClose(None) };
                            if let Some(error) = error {
                                if cancelled {
                                    return;
                                }
                                let alert = NSAlert::new(mtm);
                                alert
                                    .setMessageText(&NSString::from_str(&translation::_g("Error")));
                                alert.setInformativeText(&NSString::from_str(&translation::_f(
                                    "Unable to install the update: {0}",
                                    &[error],
                                )));
                                alert.runModal();
                            }
                        });
                    });
                } else if notify_if_up_to_date {
                    let alert = NSAlert::new(mtm);
                    alert.setMessageText(&NSString::from_str(&translation::_g(
                        "No Update Available",
                    )));
                    alert.setInformativeText(&NSString::from_str(&translation::_f(
                        "You are running the latest version of {0}.",
                        &[info::APP_ENGLISH_SHORT_NAME],
                    )));
                    alert.runModal();
                }
            });
        });
    }

    pub fn close_folder(&self) {
        let mut controller = self.ivars().controller.borrow_mut();
        controller.close_folder();
        self.window().unwrap().setSubtitle(ns_string!(""));
        self.ivars()
            .controls
            .get()
            .unwrap()
            .tab_view
            .selectTabViewItemAtIndex(HomePage::page_index());
    }

    pub fn open_folder(&self) {
        let open_panel = NSOpenPanel::new(self.mtm());
        open_panel.setCanChooseFiles(false);
        open_panel.setCanChooseDirectories(true);
        open_panel.setAllowsMultipleSelection(false);
        if open_panel.runModal() == NSModalResponseOK {
            let mut controller = self.ivars().controller.borrow_mut();
            let path = open_panel.URLs().firstObject().unwrap().path().unwrap();
            match controller.open_folder(path.to_string()) {
                Ok(files) => {
                    self.window().unwrap().setSubtitle(&path);
                    let controls = self.ivars().controls.get().unwrap();
                    controls.folder_page.show_folder(&path, files.len());
                    controls
                        .tab_view
                        .selectTabViewItemAtIndex(FolderPage::page_index());
                }
                Err(error) => {
                    let alert = NSAlert::new(self.mtm());
                    alert.setMessageText(&NSString::from_str(&translation::_g("Error")));
                    alert.setInformativeText(&NSString::from_str(&translation::_f(
                        "Unable to open folder: {0}",
                        &[error.to_string()],
                    )));
                    alert.runModal();
                }
            }
        }
    }

    pub fn show(&self) {
        unsafe { self.showWindow(None) };
    }
}
