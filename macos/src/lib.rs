mod app;

use crate::app::Delegate;
use objc2::MainThreadMarker;
use objc2::runtime::ProtocolObject;
use objc2_app_kit::NSApplication;

pub fn run() {
    let mtm = MainThreadMarker::new().unwrap();
    let app = NSApplication::sharedApplication(mtm);
    let delegate = Delegate::new(mtm);
    app.setDelegate(Some(ProtocolObject::from_ref(&*delegate)));
    app.run();
}
