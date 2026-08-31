use crate::helpers::EasyLayout;
use objc2::rc::Retained;
use objc2::runtime::{AnyObject, NSObject, Sel};
use objc2::{ClassType, MainThreadOnly, define_class, msg_send};
use objc2_app_kit::{
    NSApplication, NSButton, NSColor, NSFont, NSImageView, NSLayoutAttribute, NSStackView,
    NSTextAlignment, NSTextField, NSUserInterfaceLayoutOrientation, NSView, NSViewController,
};
use objc2_foundation::{MainThreadMarker, NSArray, NSObjectProtocol, NSRect, NSString};
use shared::translation;

#[derive(Debug, Default)]
pub struct HomePageState;

define_class!(
    #[derive(Debug)]
    #[unsafe(super = NSViewController)]
    #[thread_kind = MainThreadOnly]
    #[ivars = HomePageState]
    pub struct HomePage;

    unsafe impl NSObjectProtocol for HomePage {}
);

impl HomePage {
    pub fn new(
        mtm: MainThreadMarker,
        greeting: &str,
        target: Option<&AnyObject>,
        action: Sel,
    ) -> Retained<Self> {
        let this = Self::alloc(mtm).set_ivars(HomePageState);
        let this: Retained<Self> = unsafe {
            msg_send![super(this), initWithNibName: std::ptr::null::<NSObject>(), bundle: std::ptr::null::<NSObject>()]
        };
        let view = NSView::initWithFrame(NSView::alloc(mtm), NSRect::ZERO);
        let icon_view = NSImageView::imageViewWithImage(
            &NSApplication::sharedApplication(mtm)
                .applicationIconImage()
                .unwrap(),
            mtm,
        );
        icon_view.set_size(64.0, 64.0);
        let title_label = NSTextField::labelWithString(&NSString::from_str(greeting), mtm);
        title_label.setFont(Some(&NSFont::boldSystemFontOfSize(
            NSFont::systemFontSize() * 2.0,
        )));
        title_label.setAlignment(NSTextAlignment::Center);
        let description_label = NSTextField::labelWithString(
            &NSString::from_str(&translation::_g("Open a folder to get started")),
            mtm,
        );
        description_label.setTextColor(Some(&NSColor::secondaryLabelColor()));
        description_label.setAlignment(NSTextAlignment::Center);
        let open_button = unsafe {
            NSButton::buttonWithTitle_target_action(
                &NSString::from_str(&translation::_g("Open")),
                target,
                Some(action),
                mtm,
            )
        };
        open_button.setKeyEquivalent(&NSString::from_str("\r"));
        open_button.setToolTip(Some(&NSString::from_str(&translation::_g(
            "Open Folder (⌘O)",
        ))));
        let stack_view = NSStackView::stackViewWithViews(
            &NSArray::from_slice(&[
                &icon_view as &NSView,
                &title_label as &NSView,
                &description_label as &NSView,
                &open_button as &NSView,
            ]),
            mtm,
        );
        stack_view.setOrientation(NSUserInterfaceLayoutOrientation::Vertical);
        stack_view.setAlignment(NSLayoutAttribute::CenterX);
        stack_view.setSpacing(12.0);
        view.addSubview(&stack_view);
        stack_view.constrain_center(&view);
        this.setView(&view);
        this
    }
}
