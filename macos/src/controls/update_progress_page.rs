use crate::helpers::EasyLayout;
use objc2::rc::Retained;
use objc2::runtime::NSObject;
use objc2::{DefinedClass, MainThreadOnly, define_class, msg_send};
use objc2_app_kit::{
    NSColor, NSFont, NSLayoutAttribute, NSProgressIndicator, NSProgressIndicatorStyle, NSStackView,
    NSTextAlignment, NSTextField, NSUserInterfaceLayoutOrientation, NSView, NSViewController,
};
use objc2_foundation::{MainThreadMarker, NSArray, NSObjectProtocol, NSRect, NSString};
use shared::translation;
use std::cell::OnceCell;

#[derive(Debug)]
struct UpdateProgressPageControls {
    progress_indicator: Retained<NSProgressIndicator>,
    detail_label: Retained<NSTextField>,
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
    pub fn new(mtm: MainThreadMarker) -> Retained<Self> {
        let this = Self::alloc(mtm).set_ivars(UpdateProgressPageState::default());
        let this: Retained<Self> = unsafe {
            msg_send![super(this), initWithNibName: std::ptr::null::<NSObject>(), bundle: std::ptr::null::<NSObject>()]
        };
        let view = NSView::initWithFrame(NSView::alloc(mtm), NSRect::ZERO);
        let title_label = NSTextField::labelWithString(
            &NSString::from_str(&translation::_g("Downloading Update")),
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
        let detail_label = NSTextField::labelWithString(&NSString::from_str(""), mtm);
        detail_label.setTextColor(Some(&NSColor::secondaryLabelColor()));
        detail_label.setAlignment(NSTextAlignment::Center);
        let stack_view = NSStackView::stackViewWithViews(
            &NSArray::from_slice(&[
                &title_label as &NSView,
                &progress_indicator as &NSView,
                &detail_label as &NSView,
            ]),
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
                progress_indicator,
                detail_label,
            })
            .unwrap();
        this.setView(&view);
        this
    }

    pub fn set_progress(&self, downloaded: u64, total: u64) {
        let controls = self.ivars().controls.get().unwrap();
        if total > 0 {
            controls.progress_indicator.setIndeterminate(false);
            controls
                .progress_indicator
                .setDoubleValue(downloaded as f64 / total as f64);
            controls
                .detail_label
                .setStringValue(&NSString::from_str(&translation::_f(
                    "{0}%",
                    &[(downloaded * 100 / total).to_string()],
                )));
        } else {
            controls.progress_indicator.setIndeterminate(true);
            unsafe { controls.progress_indicator.startAnimation(None) };
            controls
                .detail_label
                .setStringValue(&NSString::from_str(&translation::_g("Downloading…")));
        }
    }
}
