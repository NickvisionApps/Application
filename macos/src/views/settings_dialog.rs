use objc2::ffi::NSInteger;
use objc2::rc::Retained;
use objc2::runtime::{AnyObject, ProtocolObject};
use objc2::{ClassType, DefinedClass, MainThreadOnly, define_class, msg_send, sel};
use objc2_app_kit::{
    NSAppearance, NSAppearanceNameAqua, NSAppearanceNameDarkAqua, NSApplication,
    NSBackingStoreType, NSGridCellPlacement, NSGridView, NSImage, NSLayoutConstraint,
    NSPopUpButton, NSTabView, NSTabViewItem, NSTabViewType, NSTextField, NSToolbar,
    NSToolbarDelegate, NSToolbarDisplayMode, NSToolbarItem, NSToolbarItemIdentifier, NSView,
    NSWindow, NSWindowController, NSWindowDelegate, NSWindowStyleMask, NSWindowToolbarStyle,
};
use objc2_foundation::{
    MainThreadMarker, NSArray, NSObjectProtocol, NSPoint, NSRect, NSSize, NSString, ns_string,
};
use shared::{AppState, ApplicationTheme, Translator};
use std::cell::{OnceCell, RefCell};
use std::rc::Rc;

#[derive(Debug)]
pub struct SettingsDialogState {
    state: Rc<RefCell<AppState>>,
    tab_view: OnceCell<Retained<NSTabView>>,
    theme_popup_button: OnceCell<Retained<NSPopUpButton>>,
    language_popup_button: OnceCell<Retained<NSPopUpButton>>,
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
            let state_ref = self.ivars().state.borrow();
            let (label, index) = if &*item.itemIdentifier() == ns_string!("General") {
                (state_ref.translator()._g("General"), 0)
            } else {
                (state_ref.translator()._g("Advanced"), 1)
            };
            if let Some(window) = self.window() {
                window.setTitle(&NSString::from_str(&label));
            }
            if let Some(tab_view) = self.ivars().tab_view.get() {
                tab_view.selectTabViewItemAtIndex(index);
            }
        }

        #[unsafe(method(popupButtonChanged:))]
        fn popup_button_changed(&self, _sender: Option<&AnyObject>) {
            let appearance = match self.ivars().theme_popup_button.get().unwrap().indexOfSelectedItem() {
                0 => unsafe { NSAppearance::appearanceNamed(NSAppearanceNameAqua) },
                1 => unsafe { NSAppearance::appearanceNamed(NSAppearanceNameDarkAqua) },
                _ => None,
            };
            NSApplication::sharedApplication(self.mtm()).setAppearance(appearance.as_deref());
            let mut state_ref = self.ivars().state.borrow_mut();
            let configuration = state_ref.configuration_mut();
            configuration.set_theme(match self.ivars().theme_popup_button.get().unwrap().indexOfSelectedItem() {
                0 => ApplicationTheme::Light,
                1 => ApplicationTheme::Dark,
                _ => ApplicationTheme::System,
            });
            configuration.set_translation_language(Translator::available_languages()
                .get(self
                .ivars()
                .language_popup_button
                .get()
                .unwrap()
                .indexOfSelectedItem() as usize)
                .cloned()
            .unwrap_or_default());
        }
    }

    unsafe impl NSToolbarDelegate for SettingsDialog {
        #[unsafe(method_id(toolbarDefaultItemIdentifiers:))]
        fn toolbar_default_item_identifiers(
            &self,
            toolbar: &NSToolbar,
        ) -> Retained<NSArray<NSToolbarItemIdentifier>> {
            NSArray::from_retained_slice(&[NSString::from_str("General"), NSString::from_str("Advanced")])
        }

        #[unsafe(method_id(toolbarAllowedItemIdentifiers:))]
        fn toolbar_allowed_item_identifiers(
            &self,
            toolbar: &NSToolbar,
        ) -> Retained<NSArray<NSToolbarItemIdentifier>> {
            NSArray::from_retained_slice(&[NSString::from_str("General"), NSString::from_str("Advanced")])
        }

        #[unsafe(method_id(toolbarSelectableItemIdentifiers:))]
        fn toolbar_selectable_item_identifiers(
            &self,
            toolbar: &NSToolbar,
        ) -> Retained<NSArray<NSToolbarItemIdentifier>> {
            NSArray::from_retained_slice(&[NSString::from_str("General"), NSString::from_str("Advanced")])
        }

        #[unsafe(method_id(toolbar:itemForItemIdentifier:willBeInsertedIntoToolbar:))]
        fn toolbar_item_for_item_identifier(
            &self,
            toolbar: &NSToolbar,
            item_identifier: &NSToolbarItemIdentifier,
            will_be_inserted: bool,
        ) -> Option<Retained<NSToolbarItem>> {
            let state_ref = self.ivars().state.borrow();
            let (label, symbol) = if item_identifier == ns_string!("General") {
                (state_ref.translator()._g("General"), "gearshape")
            } else {
                (state_ref.translator()._g("Advanced"), "slider.horizontal.3")
            };
            let item = NSToolbarItem::initWithItemIdentifier(NSToolbarItem::alloc(self.mtm()), item_identifier);
            item.setLabel(&NSString::from_str(&label));
            if let Some(image) = NSImage::imageWithSystemSymbolName_accessibilityDescription(&NSString::from_str(symbol), None) {
                item.setImage(Some(&image));
            }
            unsafe {
                item.setTarget(Some(self.as_super().as_super()));
                item.setAction(Some(sel!(toolbarItemClicked:)));
            }
            Some(item)
        }
    }

    unsafe impl NSObjectProtocol for SettingsDialog {}

    unsafe impl NSWindowDelegate for SettingsDialog {
        #[unsafe(method(windowWillClose:))]
        fn window_will_close(&self, _sender: &NSWindow) {
            let state_ref = self.ivars().state.borrow();
            state_ref.configuration().save().unwrap();
        }
    }
);

impl SettingsDialogState {
    pub fn new(state: Rc<RefCell<AppState>>) -> Self {
        SettingsDialogState {
            state,
            tab_view: OnceCell::new(),
            theme_popup_button: OnceCell::new(),
            language_popup_button: OnceCell::new(),
        }
    }
}

impl SettingsDialog {
    pub fn new(mtm: MainThreadMarker, state: Rc<RefCell<AppState>>) -> Retained<Self> {
        let this = Self::alloc(mtm).set_ivars(SettingsDialogState::new(state));
        let this: Retained<Self> = unsafe { msg_send![super(this), init] };
        let state_ref = this.ivars().state.borrow();
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
            tab_view.setTranslatesAutoresizingMaskIntoConstraints(false);
            tab_view.setTabViewType(NSTabViewType::NoTabsNoBorder);
            let general_view = NSView::new(mtm);
            let theme_label = NSTextField::labelWithString(
                &NSString::from_str(&state_ref.translator()._g("Theme:")),
                mtm,
            );
            let theme_popup_button = NSPopUpButton::new(mtm);
            theme_popup_button
                .addItemWithTitle(&NSString::from_str(&state_ref.translator()._g("Light")));
            theme_popup_button
                .addItemWithTitle(&NSString::from_str(&state_ref.translator()._g("Dark")));
            theme_popup_button
                .addItemWithTitle(&NSString::from_str(&state_ref.translator()._g("System")));
            theme_popup_button.selectItemAtIndex(match state_ref.configuration().theme() {
                ApplicationTheme::Light => 0,
                ApplicationTheme::Dark => 1,
                ApplicationTheme::System => 2,
            });
            let language_label = NSTextField::labelWithString(
                &NSString::from_str(&state_ref.translator()._g("Translation Language:")),
                mtm,
            );
            let language_popup_button = NSPopUpButton::new(mtm);
            for language in Translator::available_languages() {
                language_popup_button.addItemWithTitle(&NSString::from_str(language));
            }
            language_popup_button.selectItemAtIndex(
                Translator::available_languages()
                    .iter()
                    .position(|language| language == state_ref.translator().language())
                    .unwrap_or(0) as NSInteger,
            );
            unsafe {
                theme_popup_button.setTarget(Some(this.as_super().as_super()));
                theme_popup_button.setAction(Some(sel!(popupButtonChanged:)));
                language_popup_button.setTarget(Some(this.as_super().as_super()));
                language_popup_button.setAction(Some(sel!(popupButtonChanged:)));
            }
            let general_grid_view = NSGridView::gridViewWithViews(
                &NSArray::from_retained_slice(&[
                    NSArray::from_slice(&[&theme_label as &NSView, &theme_popup_button as &NSView]),
                    NSArray::from_slice(&[
                        &language_label as &NSView,
                        &language_popup_button as &NSView,
                    ]),
                ]),
                mtm,
            );
            general_grid_view.setTranslatesAutoresizingMaskIntoConstraints(false);
            general_grid_view
                .columnAtIndex(0)
                .setXPlacement(NSGridCellPlacement::Trailing);
            general_grid_view.setYPlacement(NSGridCellPlacement::Center);
            general_view.addSubview(&general_grid_view);
            NSLayoutConstraint::activateConstraints(&NSArray::from_retained_slice(&[
                general_grid_view
                    .centerXAnchor()
                    .constraintEqualToAnchor(&general_view.centerXAnchor()),
                general_grid_view
                    .topAnchor()
                    .constraintEqualToAnchor_constant(&general_view.topAnchor(), 100.0),
            ]));
            let general_tab = NSTabViewItem::new();
            general_tab.setView(Some(&general_view));
            tab_view.addTabViewItem(&general_tab);
            let advanced_view = NSView::new(mtm);
            let advanced_tab = NSTabViewItem::new();
            advanced_tab.setView(Some(&advanced_view));
            tab_view.addTabViewItem(&advanced_tab);
            tab_view.selectTabViewItemAtIndex(0);
            content_view.addSubview(&tab_view);
            NSLayoutConstraint::activateConstraints(&NSArray::from_retained_slice(&[
                tab_view
                    .leadingAnchor()
                    .constraintEqualToAnchor(&content_view.leadingAnchor()),
                tab_view
                    .trailingAnchor()
                    .constraintEqualToAnchor(&content_view.trailingAnchor()),
                tab_view
                    .topAnchor()
                    .constraintEqualToAnchor(&content_view.topAnchor()),
                tab_view
                    .bottomAnchor()
                    .constraintEqualToAnchor(&content_view.bottomAnchor()),
            ]));
            this.ivars().tab_view.set(tab_view).unwrap();
            this.ivars()
                .theme_popup_button
                .set(theme_popup_button)
                .unwrap();
            this.ivars()
                .language_popup_button
                .set(language_popup_button)
                .unwrap();
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
