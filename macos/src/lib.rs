mod app;
mod views;

use crate::app::Delegate;
use objc2::MainThreadMarker;
use objc2::runtime::ProtocolObject;
use objc2_app_kit::NSApplication;

pub fn run() {
    let mtm = MainThreadMarker::new().unwrap();
    NSApplication::sharedApplication(mtm)
        .setDelegate(Some(ProtocolObject::from_ref(&*Delegate::new(mtm))));
    NSApplication::sharedApplication(mtm).run();
}
