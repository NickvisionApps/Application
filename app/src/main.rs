#[cfg(target_os = "windows")]
#[tokio::main]
async fn main() {
    rustls::crypto::ring::default_provider()
        .install_default()
        .unwrap();
    windows::run();
}

#[cfg(target_os = "macos")]
#[tokio::main]
async fn main() {
    rustls::crypto::ring::default_provider()
        .install_default()
        .unwrap();
    macos::run();
}

#[cfg(target_os = "linux")]
#[tokio::main]
async fn main() {
    rustls::crypto::ring::default_provider()
        .install_default()
        .unwrap();
    linux::run();
}

#[cfg(not(any(target_os = "windows", target_os = "macos", target_os = "linux")))]
fn main() {
    panic!("Unsupported target OS. Supported targets are Windows, macOS, and Linux.");
}
