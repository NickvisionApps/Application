use crate::helpers::{EasyLayout, EasyMenu, EasyToolbarItem};
use objc2::rc::Retained;
use objc2::runtime::{AnyObject, ProtocolObject};
use objc2::{ClassType, DefinedClass, MainThreadOnly, define_class, msg_send, sel};
use objc2_app_kit::{
    NSAppearance, NSAppearanceNameAqua, NSAppearanceNameDarkAqua, NSApplication,
    NSBackingStoreType, NSButton, NSColor, NSControlStateValueOff, NSControlStateValueOn, NSFont,
    NSGridCellPlacement, NSGridView, NSMenu, NSPopUpButton, NSTabView, NSTabViewItem,
    NSTabViewType, NSTextField, NSToolbar, NSToolbarDelegate, NSToolbarDisplayMode, NSToolbarItem,
    NSToolbarItemIdentifier, NSView, NSWindow, NSWindowController, NSWindowDelegate,
    NSWindowStyleMask, NSWindowToolbarStyle,
};
use objc2_foundation::{
    MainThreadMarker, NSArray, NSNotification, NSObjectProtocol, NSPoint, NSRect, NSSize, NSString,
    ns_string,
};
use shared::{config::ApplicationTheme, controller::AppController, translation};
use std::{
    cell::{OnceCell, RefCell},
    rc::Rc,
};

#[derive(Debug)]
pub struct SettingsDialogState {
    controller: Rc<RefCell<AppController>>,
    tab_view: OnceCell<Retained<NSTabView>>,
    theme_popup_button: OnceCell<Retained<NSPopUpButton>>,
    language_popup_button: OnceCell<Retained<NSPopUpButton>>,
    preview_updates_checkbox: OnceCell<Retained<NSButton>>,
}

define_class!(
    #[derive(Debug)]
    #[unsafe(super = NSWindowController)]
    #[thread_kind = MainThreadOnly]
    #[ivars = SettingsDialogState]
    pub struct SettingsDialog;

    impl SettingsDialog {
        #[unsafe(method(toolbarItemClicked:))]
        fn toolbar_item_clicked(&self, sender: Option<&AnyObject>) {
            let item = sender
                .unwrap()
                .downcast_ref::<NSToolbarItem>()
                .unwrap();
            if let Some(window) = self.window() && let Some(tab_view) = self.ivars().tab_view.get() {
                if item.itemIdentifier() == NSToolbarItemIdentifier::from_str("Advanced") {
                    window.setTitle(&NSString::from_str(&translation::_g("Advanced")));
                    tab_view.selectTabViewItemAtIndex(1);
                } else if item.itemIdentifier() == NSToolbarItemIdentifier::from_str("General") {
                    window.setTitle(&NSString::from_str(&translation::_g("General")));
                    tab_view.selectTabViewItemAtIndex(0);
                }
            }
        }

        #[unsafe(method(popupButtonChanged:))]
        fn popup_button_changed(&self, _sender: Option<&AnyObject>) {
            self.update_configuration();
        }

        #[unsafe(method(checkboxChanged:))]
        fn checkbox_changed(&self, _sender: Option<&AnyObject>) {
            self.update_configuration();
        }
    }

    unsafe impl NSToolbarDelegate for SettingsDialog {
        #[unsafe(method_id(toolbarDefaultItemIdentifiers:))]
        fn toolbar_default_item_identifiers(
            &self,
            _toolbar: &NSToolbar,
        ) -> Retained<NSArray<NSToolbarItemIdentifier>> {
            NSArray::from_retained_slice(&[
                NSToolbarItemIdentifier::from_str("General"),
                NSToolbarItemIdentifier::from_str("Advanced")
            ])
        }

        #[unsafe(method_id(toolbarAllowedItemIdentifiers:))]
        fn toolbar_allowed_item_identifiers(
            &self,
            _toolbar: &NSToolbar,
        ) -> Retained<NSArray<NSToolbarItemIdentifier>> {
            NSArray::from_retained_slice(&[
                NSToolbarItemIdentifier::from_str("General"),
                NSToolbarItemIdentifier::from_str("Advanced")
            ])
        }

        #[unsafe(method_id(toolbarSelectableItemIdentifiers:))]
        fn toolbar_selectable_item_identifiers(
            &self,
            _toolbar: &NSToolbar,
        ) -> Retained<NSArray<NSToolbarItemIdentifier>> {
            NSArray::from_retained_slice(&[
                NSToolbarItemIdentifier::from_str("General"),
                NSToolbarItemIdentifier::from_str("Advanced")
            ])
        }

        #[unsafe(method_id(toolbar:itemForItemIdentifier:willBeInsertedIntoToolbar:))]
        fn toolbar_item_for_item_identifier(
            &self,
            toolbar: &NSToolbar,
            item_identifier: &NSToolbarItemIdentifier,
            will_be_inserted: bool,
        ) -> Option<Retained<NSToolbarItem>> {
            if item_identifier == ns_string!("Advanced") {
                NSToolbarItem::init_easy(
                    self.mtm(),
                    item_identifier,
                    translation::_g("Advanced"),
                    "slider.horizontal.3",
                    translation::_g("Advanced"),
                    Some(self.as_super().as_super()),
                    sel!(toolbarItemClicked:)
                )
            } else if item_identifier == ns_string!("General") {
                NSToolbarItem::init_easy(
                    self.mtm(),
                    item_identifier,
                    translation::_g("General"),
                    "gearshape",
                    translation::_g("General"),
                    Some(self.as_super().as_super()),
                    sel!(toolbarItemClicked:)
                )
            } else {
                None
            }
        }
    }

    unsafe impl NSObjectProtocol for SettingsDialog {}

    unsafe impl NSWindowDelegate for SettingsDialog {
        #[unsafe(method(windowWillClose:))]
        fn window_will_close(&self, _notification: &NSNotification) {
            let controller = self.ivars().controller.borrow();
            controller.save().unwrap();
        }
    }
);

impl SettingsDialogState {
    pub fn new(controller: Rc<RefCell<AppController>>) -> Self {
        SettingsDialogState {
            controller,
            tab_view: OnceCell::new(),
            theme_popup_button: OnceCell::new(),
            language_popup_button: OnceCell::new(),
            preview_updates_checkbox: OnceCell::new(),
        }
    }
}

impl SettingsDialog {
    pub fn new(mtm: MainThreadMarker, controller: Rc<RefCell<AppController>>) -> Retained<Self> {
        let this = Self::alloc(mtm).set_ivars(SettingsDialogState::new(controller));
        let this: Retained<Self> = unsafe { msg_send![super(this), init] };
        let controller = this.ivars().controller.borrow();
        let toolbar =
            NSToolbar::initWithIdentifier(NSToolbar::alloc(mtm), &NSString::from_str("Toolbar"));
        toolbar.setDelegate(Some(ProtocolObject::from_ref(&*this)));
        toolbar.setDisplayMode(NSToolbarDisplayMode::IconAndLabel);
        toolbar.setSelectedItemIdentifier(Some(ns_string!("General")));
        let window = unsafe {
            NSWindow::initWithContentRect_styleMask_backing_defer(
                NSWindow::alloc(mtm),
                NSRect::new(NSPoint::new(0.0, 0.0), NSSize::new(600.0, 400.0)),
                NSWindowStyleMask::Titled
                    | NSWindowStyleMask::Closable
                    | NSWindowStyleMask::FullSizeContentView,
                NSBackingStoreType::Buffered,
                false,
            )
        };
        window.setDelegate(Some(ProtocolObject::from_ref(&*this)));
        unsafe { window.setReleasedWhenClosed(false) };
        window.setTitle(&NSString::from_str(&translation::_g("General")));
        window.setTitlebarAppearsTransparent(true);
        window.setToolbar(Some(&toolbar));
        window.setToolbarStyle(NSWindowToolbarStyle::Preference);
        if let Some(content_view) = window.contentView() {
            let tab_view = NSTabView::new(mtm);
            tab_view.setTabViewType(NSTabViewType::NoTabsNoBorder);
            let general_view = NSView::new(mtm);
            let theme_label =
                NSTextField::labelWithString(&NSString::from_str(&translation::_g("Theme:")), mtm);
            let theme_menu = NSMenu::new(mtm);
            theme_menu.add_item_easy(translation::_g("Light"), None, "", None, None);
            theme_menu.add_item_easy(translation::_g("Dark"), None, "", None, None);
            theme_menu.add_item_easy(translation::_g("System"), None, "", None, None);
            let theme_popup_button = unsafe {
                NSPopUpButton::popUpButtonWithMenu_target_action(
                    &theme_menu,
                    Some(this.as_super().as_super()),
                    Some(sel!(popupButtonChanged:)),
                )
            };
            theme_popup_button.selectItemAtIndex(
                ApplicationTheme::ALL
                    .iter()
                    .position(|theme| theme == controller.theme())
                    .unwrap_or(0) as isize,
            );
            let language_label = NSTextField::labelWithString(
                &NSString::from_str(&translation::_g("Translation Language:")),
                mtm,
            );
            let language_menu = NSMenu::new(mtm);
            for language in translation::available_languages() {
                language_menu.add_item_easy(language, None, "", None, None);
            }
            let language_popup_button = unsafe {
                NSPopUpButton::popUpButtonWithMenu_target_action(
                    &language_menu,
                    Some(this.as_super().as_super()),
                    Some(sel!(popupButtonChanged:)),
                )
            };
            language_popup_button.selectItemAtIndex(
                translation::available_languages()
                    .iter()
                    .position(|language| language == translation::language())
                    .unwrap_or(0) as isize,
            );
            let restart_notice_label = NSTextField::wrappingLabelWithString(
                &NSString::from_str(&translation::_g(
                    "An application restart is required for change to take effect",
                )),
                mtm,
            );
            restart_notice_label.setFont(Some(&NSFont::systemFontOfSize(
                NSFont::smallSystemFontSize(),
            )));
            restart_notice_label.setTextColor(Some(&NSColor::secondaryLabelColor()));
            restart_notice_label.setPreferredMaxLayoutWidth(200.0);
            let general_grid_view = NSGridView::gridViewWithViews(
                &NSArray::from_retained_slice(&[
                    NSArray::from_slice(&[&theme_label as &NSView, &theme_popup_button as &NSView]),
                    NSArray::from_slice(&[
                        &language_label as &NSView,
                        &language_popup_button as &NSView,
                    ]),
                    NSArray::from_slice(&[
                        &NSView::new(mtm) as &NSView,
                        &restart_notice_label as &NSView,
                    ]),
                ]),
                mtm,
            );
            general_grid_view
                .columnAtIndex(0)
                .setXPlacement(NSGridCellPlacement::Trailing);
            general_grid_view.setYPlacement(NSGridCellPlacement::Center);
            general_view.addSubview(&general_grid_view);
            general_grid_view.constrain_center_horizontally(&general_view, Some(100.0), None);
            let general_tab = NSTabViewItem::new();
            general_tab.setView(Some(&general_view));
            tab_view.addTabViewItem(&general_tab);
            let advanced_view = NSView::new(mtm);
            let preview_updates_checkbox = unsafe {
                NSButton::checkboxWithTitle_target_action(
                    &NSString::from_str(&translation::_g("Allow Preview Updates")),
                    Some(this.as_super().as_super()),
                    Some(sel!(checkboxChanged:)),
                    mtm,
                )
            };
            preview_updates_checkbox.setState(if controller.allow_preview_updates() {
                NSControlStateValueOn
            } else {
                NSControlStateValueOff
            });
            let advanced_grid_view = NSGridView::gridViewWithViews(
                &NSArray::from_retained_slice(&[NSArray::from_slice(&[
                    &preview_updates_checkbox as &NSView
                ])]),
                mtm,
            );
            advanced_grid_view.setYPlacement(NSGridCellPlacement::Center);
            advanced_view.addSubview(&advanced_grid_view);
            advanced_grid_view.constrain_center_horizontally(&advanced_view, Some(100.0), None);
            let advanced_tab = NSTabViewItem::new();
            advanced_tab.setView(Some(&advanced_view));
            tab_view.addTabViewItem(&advanced_tab);
            tab_view.selectTabViewItemAtIndex(0);
            content_view.addSubview(&tab_view);
            tab_view.constrain_fill(&content_view);
            this.ivars().tab_view.set(tab_view).unwrap();
            this.ivars()
                .theme_popup_button
                .set(theme_popup_button)
                .unwrap();
            this.ivars()
                .language_popup_button
                .set(language_popup_button)
                .unwrap();
            this.ivars()
                .preview_updates_checkbox
                .set(preview_updates_checkbox)
                .unwrap();
        }
        window.center();
        drop(controller);
        this.setWindow(Some(&window));
        this
    }

    pub fn show(&self) {
        unsafe { self.showWindow(None) };
    }

    fn update_configuration(&self) {
        let appearance = match self
            .ivars()
            .theme_popup_button
            .get()
            .unwrap()
            .indexOfSelectedItem()
        {
            0 => unsafe { NSAppearance::appearanceNamed(NSAppearanceNameAqua) },
            1 => unsafe { NSAppearance::appearanceNamed(NSAppearanceNameDarkAqua) },
            _ => None,
        };
        NSApplication::sharedApplication(self.mtm()).setAppearance(appearance.as_deref());
        let mut controller = self.ivars().controller.borrow_mut();
        controller.set_theme(
            ApplicationTheme::ALL
                .get(
                    self.ivars()
                        .theme_popup_button
                        .get()
                        .unwrap()
                        .indexOfSelectedItem() as usize,
                )
                .cloned()
                .unwrap_or_default(),
        );
        controller.set_translation_language(
            translation::available_languages()
                .get(
                    self.ivars()
                        .language_popup_button
                        .get()
                        .unwrap()
                        .indexOfSelectedItem() as usize,
                )
                .cloned()
                .unwrap_or_default(),
        );
        controller.set_allow_preview_updates(
            self.ivars().preview_updates_checkbox.get().unwrap().state() == NSControlStateValueOn,
        );
    }
}
