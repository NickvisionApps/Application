use objc2::MainThreadOnly;
use objc2::rc::Retained;
use objc2::runtime::{AnyObject, Sel};
use objc2_app_kit::{NSApplication, NSImage, NSMenu, NSMenuItem};
use objc2_foundation::{NSString, ns_string};

pub trait EasyMenu {
    fn add_item_easy(
        &self,
        label: impl AsRef<str>,
        image: Option<&str>,
        char_code: &str,
        target: Option<&AnyObject>,
        action: Option<Sel>,
    ) -> Retained<NSMenuItem>;

    fn add_separator_item(&self) -> Retained<NSMenuItem>;

    fn add_services_item(&self, label: impl AsRef<str>) -> Retained<NSMenuItem>;
}

impl EasyMenu for Retained<NSMenu> {
    fn add_item_easy(
        &self,
        label: impl AsRef<str>,
        image: Option<&str>,
        char_code: &str,
        target: Option<&AnyObject>,
        action: Option<Sel>,
    ) -> Retained<NSMenuItem> {
        unsafe {
            let item = self.addItemWithTitle_action_keyEquivalent(
                &NSString::from_str(label.as_ref()),
                action,
                &NSString::from_str(char_code),
            );
            if target.is_some() {
                item.setTarget(target)
            }
            if let Some(image) = image
                && let Some(image) = NSImage::imageWithSystemSymbolName_accessibilityDescription(
                    &NSString::from_str(image),
                    None,
                )
            {
                item.setImage(Some(&image));
            }
            item
        }
    }

    fn add_separator_item(&self) -> Retained<NSMenuItem> {
        let item = NSMenuItem::separatorItem(self.mtm());
        self.addItem(&item);
        item
    }

    fn add_services_item(&self, label: impl AsRef<str>) -> Retained<NSMenuItem> {
        unsafe {
            let item = self.addItemWithTitle_action_keyEquivalent(
                &NSString::from_str(label.as_ref()),
                None,
                ns_string!(""),
            );
            let menu = NSMenu::initWithTitle(
                NSMenu::alloc(self.mtm()),
                &NSString::from_str(label.as_ref()),
            );
            item.setSubmenu(Some(&menu));
            NSApplication::sharedApplication(self.mtm()).setServicesMenu(Some(&menu));
            item
        }
    }
}
