use objc2::MainThreadOnly;
use objc2::rc::Retained;
use objc2::runtime::{AnyObject, Sel};
use objc2_app_kit::{NSButton, NSImage, NSToolbarItem, NSToolbarItemIdentifier};
use objc2_foundation::{MainThreadMarker, NSString};

pub trait EasyToolbarItem {
    #[allow(clippy::too_many_arguments)]
    fn init_easy(
        mtm: MainThreadMarker,
        id: &NSToolbarItemIdentifier,
        label: impl AsRef<str>,
        image: &str,
        tooltip: impl AsRef<str>,
        popover: bool,
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
        popover: bool,
        target: Option<&AnyObject>,
        action: Sel,
    ) -> Option<Retained<Self>> {
        let item = NSToolbarItem::initWithItemIdentifier(NSToolbarItem::alloc(mtm), id);
        let image = NSImage::imageWithSystemSymbolName_accessibilityDescription(
            &NSString::from_str(image),
            None,
        );
        item.setLabel(&NSString::from_str(label.as_ref()));
        item.setPaletteLabel(&NSString::from_str(label.as_ref()));
        if popover {
            let button = if let Some(image) = image {
                unsafe {
                    NSButton::buttonWithImage_target_action(&image, target, Some(action), mtm)
                }
            } else {
                unsafe {
                    NSButton::buttonWithTitle_target_action(
                        &NSString::from_str(label.as_ref()),
                        target,
                        Some(action),
                        mtm,
                    )
                }
            };
            item.setView(Some(&button));
        } else {
            if let Some(image) = image {
                item.setImage(Some(&image))
            }
            unsafe {
                item.setTarget(target);
                item.setAction(Some(action));
            }
        }
        item.setToolTip(Some(&NSString::from_str(tooltip.as_ref())));
        Some(item)
    }
}
