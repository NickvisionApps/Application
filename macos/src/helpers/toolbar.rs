use objc2::MainThreadOnly;
use objc2::rc::Retained;
use objc2::runtime::{AnyObject, Sel};
use objc2_app_kit::{NSImage, NSToolbarItem, NSToolbarItemIdentifier};
use objc2_foundation::{MainThreadMarker, NSString};

pub trait EasyToolbarItem {
    fn init_easy(
        mtm: MainThreadMarker,
        id: &NSToolbarItemIdentifier,
        label: impl AsRef<str>,
        image: &str,
        tooltip: impl AsRef<str>,
        target: Option<&AnyObject>,
        action: Sel,
    ) -> Option<Retained<NSToolbarItem>>;
}

impl EasyToolbarItem for NSToolbarItem {
    fn init_easy(
        mtm: MainThreadMarker,
        id: &NSToolbarItemIdentifier,
        label: impl AsRef<str>,
        image: &str,
        tooltip: impl AsRef<str>,
        target: Option<&AnyObject>,
        action: Sel,
    ) -> Option<Retained<Self>> {
        let item = NSToolbarItem::initWithItemIdentifier(NSToolbarItem::alloc(mtm), id);
        item.setLabel(&NSString::from_str(label.as_ref()));
        item.setPaletteLabel(&NSString::from_str(label.as_ref()));
        if let Some(image) = NSImage::imageWithSystemSymbolName_accessibilityDescription(
            &NSString::from_str(image),
            None,
        ) {
            item.setImage(Some(&image))
        }
        item.setToolTip(Some(&NSString::from_str(tooltip.as_ref())));
        unsafe {
            item.setTarget(target);
            item.setAction(Some(action));
        }
        Some(item)
    }
}
