use crate::helpers::EasyLayout;
use objc2::rc::Retained;
use objc2::runtime::{AnyObject, NSObject, Sel};
use objc2::{DefinedClass, MainThreadOnly, define_class, msg_send};
use objc2_app_kit::{
    NSButton, NSFont, NSImage, NSLayoutAttribute, NSProgressIndicator, NSProgressIndicatorStyle,
    NSStackView, NSTextAlignment, NSTextField, NSUserInterfaceLayoutOrientation, NSView,
    NSViewController,
};
use objc2_foundation::{MainThreadMarker, NSArray, NSObjectProtocol, NSRect, NSString};
use shared::translation;
use std::cell::OnceCell;

#[derive(Debug)]
struct UpdateProgressPageControls {
    title_label: Retained<NSTextField>,
    progress_indicator: Retained<NSProgressIndicator>,
    progress_row: Retained<NSStackView>,
}

#[derive(Debug, Default)]
pub struct UpdateProgressPageState {
    controls: OnceCell<UpdateProgressPageControls>,
}

define_class!(
    #[derive(Debug)]
    #[unsafe(super = NSViewController)]
    #[thread_kind = MainThreadOnly]
    #[ivars = UpdateProgressPageState]
    pub struct UpdateProgressPage;

    unsafe impl NSObjectProtocol for UpdateProgressPage {}
);

impl UpdateProgressPage {
    pub fn new(mtm: MainThreadMarker, target: Option<&AnyObject>, action: Sel) -> Retained<Self> {
        let this = Self::alloc(mtm).set_ivars(UpdateProgressPageState::default());
        let this: Retained<Self> = unsafe {
            msg_send![super(this), initWithNibName: std::ptr::null::<NSObject>(), bundle: std::ptr::null::<NSObject>()]
        };
        let view = NSView::initWithFrame(NSView::alloc(mtm), NSRect::ZERO);
        let title_label = NSTextField::labelWithString(
            &NSString::from_str(&translation::_g("No update in progress")),
            mtm,
        );
        title_label.setFont(Some(
            &NSFont::boldSystemFontOfSize(NSFont::systemFontSize()),
        ));
        title_label.setAlignment(NSTextAlignment::Center);
        let progress_indicator = NSProgressIndicator::new(mtm);
        progress_indicator.setStyle(NSProgressIndicatorStyle::Bar);
        progress_indicator.setIndeterminate(false);
        progress_indicator.setMinValue(0.0);
        progress_indicator.setMaxValue(1.0);
        progress_indicator.setDoubleValue(0.0);
        progress_indicator
            .widthAnchor()
            .constraintEqualToConstant(220.0)
            .setActive(true);
        let cancel_button = unsafe {
            NSButton::buttonWithImage_target_action(
                &NSImage::imageWithSystemSymbolName_accessibilityDescription(
                    &NSString::from_str("xmark.circle.fill"),
                    None,
                )
                .unwrap(),
                target,
                Some(action),
                mtm,
            )
        };
        cancel_button.setBordered(false);
        cancel_button.setToolTip(Some(&NSString::from_str(&translation::_g("Cancel"))));
        let progress_row = NSStackView::stackViewWithViews(
            &NSArray::from_slice(&[&progress_indicator as &NSView, &cancel_button as &NSView]),
            mtm,
        );
        progress_row.setOrientation(NSUserInterfaceLayoutOrientation::Horizontal);
        progress_row.setSpacing(8.0);
        progress_row.setHidden(true);
        let stack_view = NSStackView::stackViewWithViews(
            &NSArray::from_slice(&[&title_label as &NSView, &progress_row as &NSView]),
            mtm,
        );
        stack_view.setOrientation(NSUserInterfaceLayoutOrientation::Vertical);
        stack_view.setAlignment(NSLayoutAttribute::CenterX);
        stack_view.setSpacing(8.0);
        view.addSubview(&stack_view);
        stack_view.constrain_fill_margin(&view, 16.0, 16.0, 16.0, 16.0);
        this.ivars()
            .controls
            .set(UpdateProgressPageControls {
                title_label,
                progress_indicator,
                progress_row,
            })
            .unwrap();
        this.setView(&view);
        this
    }

    pub fn reset(&self) {
        let controls = self.ivars().controls.get().unwrap();
        controls
            .title_label
            .setStringValue(&NSString::from_str(&translation::_g(
                "No update in progress",
            )));
        controls.progress_row.setHidden(true);
        controls.progress_indicator.setDoubleValue(0.0);
    }

    pub fn set_progress(&self, downloaded: u64, total: u64) {
        if total > 0 && downloaded >= total {
            self.reset();
            return;
        }
        let controls = self.ivars().controls.get().unwrap();
        controls
            .title_label
            .setStringValue(&NSString::from_str(&translation::_g("Downloading Update…")));
        controls.progress_row.setHidden(false);
        controls.progress_indicator.setDoubleValue(if total > 0 {
            downloaded as f64 / total as f64
        } else {
            0.0
        });
        controls
            .progress_indicator
            .setToolTip(Some(&NSString::from_str(&translation::_f(
                "{0}%",
                &[(downloaded * 100)
                    .checked_div(total)
                    .unwrap_or(0)
                    .to_string()],
            ))));
    }
}
