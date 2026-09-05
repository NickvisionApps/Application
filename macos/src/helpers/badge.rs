use crate::helpers::EasyLayout;
use objc2::rc::Retained;
use objc2_app_kit::{NSColor, NSImage, NSImageView, NSView};
use objc2_foundation::{MainThreadMarker, NSString};

pub trait EasyBadge {
    fn add_badge_to_view(&self, mtm: MainThreadMarker) -> Retained<NSImageView>;
}

impl EasyBadge for NSView {
    fn add_badge_to_view(&self, mtm: MainThreadMarker) -> Retained<NSImageView> {
        let badge = NSImageView::imageViewWithImage(
            &NSImage::imageWithSystemSymbolName_accessibilityDescription(
                &NSString::from_str("circle.fill"),
                None,
            )
            .unwrap(),
            mtm,
        );
        badge.setContentTintColor(Some(&NSColor::systemRedColor()));
        badge.set_size(8.0, 8.0);
        self.addSubview(&badge);
        badge.constrain_margin(self, None, Some(2.0), Some(2.0), None);
        badge
    }
}
