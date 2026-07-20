#[cfg(target_os = "windows")]
fn main() {
    windows::run();
}

#[cfg(target_os = "macos")]
fn main() {
    macos::run();
}

#[cfg(target_os = "linux")]
fn main() {
    linux::run();
}

#[cfg(not(any(target_os = "windows", target_os = "macos", target_os = "linux")))]
fn main() {
    panic!("Unsupported target OS. Supported targets are Windows, macOS, and Linux.");
}
