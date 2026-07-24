use objc2::rc::Retained;
use objc2::runtime::ProtocolObject;
use objc2::{DefinedClass, MainThreadOnly, define_class, msg_send};
use objc2_app_kit::{
    NSApplication, NSApplicationActivationPolicy, NSApplicationDelegate, NSAutoresizingMaskOptions,
    NSBackingStoreType, NSColor, NSFont, NSTextAlignment, NSTextField, NSWindow, NSWindowDelegate,
    NSWindowStyleMask,
};
use objc2_foundation::{
    MainThreadMarker, NSNotification, NSObject, NSObjectProtocol, NSPoint, NSRect, NSSize,
    ns_string,
};
use shared::AppState;
use std::cell::OnceCell;

#[derive(Debug, Default)]
pub struct DelegateState {
    state: AppState,
    window: OnceCell<Retained<NSWindow>>,
}

define_class!(
    #[unsafe(super = NSObject)]
    #[thread_kind = MainThreadOnly]
    #[ivars = DelegateState]
    pub struct Delegate;

    unsafe impl NSObjectProtocol for Delegate {}

    unsafe impl NSApplicationDelegate for Delegate {
        #[unsafe(method(applicationDidFinishLaunching:))]
        fn did_finish_launching(&self, notification: &NSNotification) {
            let mtm = self.mtm();
            let app = notification
                .object()
                .unwrap()
                .downcast::<NSApplication>()
                .unwrap();
            let text_field = NSTextField::labelWithString(ns_string!("Hello, World!"), mtm);
            text_field.setFrame(NSRect::new(
                NSPoint::new(5.0, 100.0),
                NSSize::new(290.0, 100.0),
            ));
            text_field.setTextColor(Some(&NSColor::colorWithSRGBRed_green_blue_alpha(
                0.0, 0.5, 0.0, 1.0,
            )));
            text_field.setAlignment(NSTextAlignment::Center);
            text_field.setFont(Some(&NSFont::systemFontOfSize(45.0)));
            text_field.setAutoresizingMask(
                NSAutoresizingMaskOptions::ViewWidthSizable
                    | NSAutoresizingMaskOptions::ViewHeightSizable,
            );
            let window = unsafe {
                NSWindow::initWithContentRect_styleMask_backing_defer(
                    NSWindow::alloc(mtm),
                    NSRect::new(NSPoint::new(0.0, 0.0), NSSize::new(300.0, 300.0)),
                    NSWindowStyleMask::Titled
                        | NSWindowStyleMask::Closable
                        | NSWindowStyleMask::Miniaturizable
                        | NSWindowStyleMask::Resizable,
                    NSBackingStoreType::Buffered,
                    false,
                )
            };
            unsafe { window.setReleasedWhenClosed(false) };
            window.setTitle(ns_string!("A window"));
            let view = window.contentView().expect("window must have content view");
            view.addSubview(&text_field);
            window.center();
            window.setContentMinSize(NSSize::new(300.0, 300.0));
            window.setDelegate(Some(ProtocolObject::from_ref(self)));
            window.makeKeyAndOrderFront(None);
            self.ivars().window.set(window).unwrap();
            app.setActivationPolicy(NSApplicationActivationPolicy::Regular);
            #[allow(deprecated)]
            app.activateIgnoringOtherApps(true);
        }
    }

    unsafe impl NSWindowDelegate for Delegate {
        #[unsafe(method(windowWillClose:))]
        fn window_will_close(&self, _notification: &NSNotification) {
            NSApplication::sharedApplication(self.mtm()).terminate(None)
        }
    }
);

impl Delegate {
    pub fn new(mtm: MainThreadMarker) -> Retained<Self> {
        let this = Self::alloc(mtm).set_ivars(DelegateState::default());
        unsafe { msg_send![super(this), init] }
    }
}
