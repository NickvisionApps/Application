#[cfg(not(any(target_os = "windows", target_os = "macos", target_os = "linux")))]
compile_error!("Application only supports Windows, macOS, and Linux");

pub mod config;
pub mod controller;
pub mod folder;
pub mod info;
pub mod translation;
pub mod update;
