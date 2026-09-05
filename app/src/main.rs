#[cfg(all(target_os = "windows", not(feature = "force-gtk-run")))]
fn main() {
    rustls::crypto::ring::default_provider()
        .install_default()
        .unwrap();
    windows::run();
}

#[cfg(all(target_os = "windows", feature = "force-gtk-run"))]
fn main() {
    rustls::crypto::ring::default_provider()
        .install_default()
        .unwrap();
    linux::run();
}

#[cfg(all(target_os = "macos", not(feature = "force-gtk-run")))]
fn main() {
    rustls::crypto::ring::default_provider()
        .install_default()
        .unwrap();
    macos::run();
}

#[cfg(all(target_os = "macos", feature = "force-gtk-run"))]
fn main() {
    rustls::crypto::ring::default_provider()
        .install_default()
        .unwrap();
    linux::run();
}

#[cfg(target_os = "linux")]
fn main() {
    rustls::crypto::ring::default_provider()
        .install_default()
        .unwrap();
    linux::run();
}

#[cfg(not(any(target_os = "windows", target_os = "macos", target_os = "linux")))]
fn main() {
    panic!("Unsupported target OS. Supported targets are Windows, macOS, and Linux.");
}
