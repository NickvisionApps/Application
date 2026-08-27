use crate::helpers::EasyLayout;
use objc2::rc::Retained;
use objc2::{ClassType, DefinedClass, MainThreadOnly, define_class, msg_send};
use objc2_app_kit::{
    NSColor, NSFont, NSFontWeightRegular, NSImage, NSImageSymbolConfiguration, NSImageView,
    NSLayoutAttribute, NSStackView, NSTextAlignment, NSTextField, NSUserInterfaceLayoutOrientation,
    NSView,
};
use objc2_foundation::{MainThreadMarker, NSArray, NSObjectProtocol, NSRect, NSString};
use shared::translation;
use std::cell::OnceCell;
use std::path::Path;

#[derive(Debug)]
struct FolderPageControls {
    title_label: Retained<NSTextField>,
    description_label: Retained<NSTextField>,
}

#[derive(Debug, Default)]
pub struct FolderPageState {
    controls: OnceCell<FolderPageControls>,
}

define_class!(
    #[derive(Debug)]
    #[unsafe(super = NSView)]
    #[thread_kind = MainThreadOnly]
    #[ivars = FolderPageState]
    pub struct FolderPage;

    unsafe impl NSObjectProtocol for FolderPage {}
);

impl FolderPage {
    pub fn new(mtm: MainThreadMarker) -> Retained<Self> {
        let this = Self::alloc(mtm).set_ivars(FolderPageState::default());
        let this: Retained<Self> = unsafe { msg_send![super(this), initWithFrame: NSRect::ZERO] };
        let icon_view = NSImageView::imageViewWithImage(
            &NSImage::imageWithSystemSymbolName_accessibilityDescription(
                &NSString::from_str("folder"),
                None,
            )
            .unwrap(),
            mtm,
        );
        icon_view.setSymbolConfiguration(Some(
            &NSImageSymbolConfiguration::configurationWithPointSize_weight(32.0, unsafe {
                NSFontWeightRegular
            }),
        ));
        icon_view.setTranslatesAutoresizingMaskIntoConstraints(false);
        icon_view
            .widthAnchor()
            .constraintEqualToConstant(40.0)
            .setActive(true);
        icon_view
            .heightAnchor()
            .constraintEqualToConstant(40.0)
            .setActive(true);
        let title_label = NSTextField::labelWithString(&NSString::from_str(""), mtm);
        title_label.setFont(Some(
            &NSFont::boldSystemFontOfSize(NSFont::systemFontSize()),
        ));
        title_label.setAlignment(NSTextAlignment::Center);
        let description_label = NSTextField::labelWithString(&NSString::from_str(""), mtm);
        description_label.setTextColor(Some(&NSColor::secondaryLabelColor()));
        description_label.setAlignment(NSTextAlignment::Center);
        let stack_view = NSStackView::stackViewWithViews(
            &NSArray::from_slice(&[
                &icon_view as &NSView,
                &title_label as &NSView,
                &description_label as &NSView,
            ]),
            mtm,
        );
        stack_view.setOrientation(NSUserInterfaceLayoutOrientation::Vertical);
        stack_view.setAlignment(NSLayoutAttribute::CenterX);
        stack_view.setSpacing(8.0);
        this.addSubview(&stack_view);
        stack_view.constrain_center(this.as_super());
        this.ivars()
            .controls
            .set(FolderPageControls {
                title_label,
                description_label,
            })
            .unwrap();
        this
    }

    pub fn show_folder(&self, path: &Path, file_count: u64) {
        let controls = self.ivars().controls.get().unwrap();
        controls
            .title_label
            .setStringValue(&NSString::from_str(path.to_str().unwrap()));
        controls
            .description_label
            .setStringValue(&NSString::from_str(&translation::_nf(
                "{0} file",
                "{0} files",
                file_count,
                &[file_count.to_string()],
            )));
    }
}
