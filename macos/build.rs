use std::env;
use std::fs;
use std::path::PathBuf;

const APP_ID: &str = "org.nickvision.application";
const APP_BUNDLE_NAME: &str = "Application.app";
const APP_DISPLAY_NAME: &str = "Application";
const APP_EXECUTABLE_NAME: &str = "app";
const MINIMUM_MACOS_VERSION: &str = "14.0";

fn main() {
    println!("cargo:rerun-if-changed=build.rs");
    println!("cargo:rerun-if-changed=../resources/org.nickvision.application.icns");
    if env::var("CARGO_CFG_TARGET_OS").ok().as_deref() != Some("macos") {
        return;
    }
    if let Err(err) = create_app_bundle() {
        panic!("failed to create macOS app bundle: {err}");
    }
}

fn create_app_bundle() -> Result<(), Box<dyn std::error::Error>> {
    let version = env::var("CARGO_PKG_VERSION")?;
    let manifest_dir = PathBuf::from(env::var("CARGO_MANIFEST_DIR")?);
    let workspace_root = manifest_dir
        .parent()
        .ok_or("macos crate must be in workspace root child")?
        .to_path_buf();
    let out_dir = PathBuf::from(env::var("OUT_DIR")?);
    let profile_dir = out_dir
        .ancestors()
        .nth(3)
        .map(PathBuf::from)
        .ok_or_else(|| {
            format!("unable to resolve target profile output from OUT_DIR: {out_dir:?}")
        })?;
    let bundle_dir = profile_dir.join(APP_BUNDLE_NAME);
    let contents_dir = bundle_dir.join("Contents");
    let macos_dir = contents_dir.join("MacOS");
    let resources_dir = contents_dir.join("Resources");
    let icon_source_path = workspace_root
        .join("resources")
        .join(format!("{APP_ID}.icns"));
    let has_icon = icon_source_path.exists();
    fs::create_dir_all(&macos_dir)?;
    fs::create_dir_all(&resources_dir)?;
    let icon_entry = if has_icon {
        format!("    <key>CFBundleIconFile</key>\n    <string>{APP_ID}.icns</string>\n")
    } else {
        String::new()
    };
    fs::write(
        contents_dir.join("Info.plist"),
        format!(
            r#"<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
<dict>
    <key>CFBundleExecutable</key>
    <string>{APP_EXECUTABLE_NAME}</string>
    <key>CFBundleIdentifier</key>
    <string>{APP_ID}</string>
    <key>CFBundleName</key>
    <string>{APP_DISPLAY_NAME}</string>
    <key>CFBundleVersion</key>
    <string>{version}</string>
    <key>CFBundleShortVersionString</key>
    <string>{version}</string>
{icon_entry}    <key>NSHighResolutionCapable</key>
    <true/>
    <key>CFBundlePackageType</key>
    <string>APPL</string>
    <key>LSMinimumSystemVersion</key>
    <string>{MINIMUM_MACOS_VERSION}</string>
    <key>NSHumanReadableCopyright</key>
    <string>Copyright © 2021-2026 Nickvision. All rights reserved.</string>
</dict>
</plist>
"#
        ),
    )?;
    if has_icon {
        fs::copy(
            icon_source_path,
            resources_dir.join(format!("{APP_ID}.icns")),
        )?;
    }
    let binary_path = profile_dir.join(APP_EXECUTABLE_NAME);
    if !binary_path.exists() {
        return Err(format!(
            "expected executable not found at {}; build app binary first",
            binary_path.display()
        )
            .into());
    }
    let bundle_binary_path = macos_dir.join(APP_EXECUTABLE_NAME);
    if fs::symlink_metadata(&bundle_binary_path).is_ok() {
        fs::remove_file(&bundle_binary_path)?;
    }
    fs::copy(&binary_path, &bundle_binary_path)?;
    for entry in fs::read_dir(&profile_dir)? {
        let entry = entry?;
        let path = entry.path();
        if !path.is_file() {
            continue;
        }
        let extension = path.extension().and_then(|e| e.to_str());
        if !matches!(extension, Some("dylib") | Some("so")) {
            continue;
        }
        let destination = macos_dir.join(entry.file_name());
        if fs::symlink_metadata(&destination).is_ok() {
            fs::remove_file(&destination)?;
        }
        fs::copy(path, destination)?;
    }
    Ok(())
}
