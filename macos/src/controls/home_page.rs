use crate::helpers::EasyLayout;
use objc2::rc::Retained;
use objc2::runtime::{AnyObject, Sel};
use objc2::{ClassType, MainThreadOnly, define_class, msg_send};
use objc2_app_kit::{
    NSApplication, NSBezelStyle, NSButton, NSColor, NSFont, NSImageView, NSLayoutAttribute,
    NSStackView, NSTextAlignment, NSTextField, NSUserInterfaceLayoutOrientation, NSView,
};
use objc2_foundation::{MainThreadMarker, NSArray, NSObjectProtocol, NSRect, NSString};
use shared::translation;

#[derive(Debug, Default)]
pub struct HomePageState;

define_class!(
    #[derive(Debug)]
    #[unsafe(super = NSView)]
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
        let this: Retained<Self> = unsafe { msg_send![super(this), initWithFrame: NSRect::ZERO] };
        let icon_view = NSImageView::imageViewWithImage(
            &NSApplication::sharedApplication(mtm)
                .applicationIconImage()
                .unwrap(),
            mtm,
        );
        icon_view.setTranslatesAutoresizingMaskIntoConstraints(false);
        icon_view
            .widthAnchor()
            .constraintEqualToConstant(64.0)
            .setActive(true);
        icon_view
            .heightAnchor()
            .constraintEqualToConstant(64.0)
            .setActive(true);
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
        open_button.setBezelStyle(NSBezelStyle::Rounded);
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
        this.addSubview(&stack_view);
        stack_view.constrain_center(this.as_super());
        this
    }
}
