use std::env;
use std::fs;
use std::path::{Path, PathBuf};
use std::process::Command;

const SHORT_NAME: &str = "application";
const XGETTEXT_KEYWORDS: &[&str] = &[
    "_g:1",
    "_f:1",
    "_n:1,2",
    "_nf:1,2",
    "_p:1c,2",
    "_pf:1c,2",
    "_np:1c,2,3",
    "_npf:1c,2,3",
];

fn main() {
    let root_dir =
        PathBuf::from(env::var("CARGO_MANIFEST_DIR").expect("CARGO_MANIFEST_DIR not set"))
            .ancestors()
            .find(|dir| dir.join("resources").join("po").join("LINGUAS").exists())
            .expect("translation generation error: unable to locate root resources/po/LINGUAS")
            .to_path_buf();
    let po_dir = root_dir.join("resources").join("po");
    let output_dir = PathBuf::from(env::var("OUT_DIR").expect("OUT_DIR not set"))
        .ancestors()
        .find(|dir| dir.file_name().is_some_and(|name| name == "build"))
        .expect("translation generation error: unable to locate Cargo build directory from OUT_DIR")
        .parent()
        .expect(
            "translation generation error: unable to locate profile output directory from OUT_DIR",
        )
        .parent()
        .expect("translation generation error: unable to locate target directory from OUT_DIR")
        .to_path_buf();
    println!(
        "cargo:rerun-if-changed={}",
        root_dir.join("src-tauri").join("src").display()
    );
    println!("cargo:rerun-if-changed={}", root_dir.join("src").display());
    println!(
        "cargo:rerun-if-changed={}",
        po_dir.join("LINGUAS").display()
    );
    let mut sources = find_files(&root_dir, &root_dir.join("src-tauri").join("src"), &["rs"]);
    sources.extend(find_files(&root_dir, &root_dir.join("src"), &["ts", "tsx"]));
    let mut xgettext_command = Command::new("xgettext");
    xgettext_command
        .current_dir(&root_dir)
        .arg("--from-code=utf-8")
        .arg("--language=C")
        .arg("--force-po")
        .arg(format!(
            "--output={}",
            po_dir.join(format!("{SHORT_NAME}.pot")).display()
        ))
        .arg("--width=80");
    for keyword in XGETTEXT_KEYWORDS {
        xgettext_command.arg(format!("--keyword={keyword}"));
    }
    xgettext_command.args(&sources);
    let status = xgettext_command
        .status()
        .unwrap_or_else(|e| panic!("translation generation error: failed to run xgettext: {e}"));
    assert!(
        status.success(),
        "translation generation error: xgettext failed with status {status}"
    );
    let locale_dir = output_dir.join("locale");
    let _ = fs::remove_dir_all(&locale_dir);
    fs::create_dir_all(&locale_dir).unwrap_or_else(|e| {
        panic!(
            "translation generation error: failed to create output directory {}: {e}",
            locale_dir.display()
        )
    });
    for language in &fs::read_to_string(po_dir.join("LINGUAS"))
        .unwrap_or_else(|e| {
            panic!(
                "translation generation error: failed to read {}: {e}",
                po_dir.join("LINGUAS").display()
            )
        })
        .lines()
        .map(str::trim)
        .filter(|line| !line.is_empty() && !line.starts_with('#'))
        .map(ToOwned::to_owned)
        .collect::<Vec<String>>()
    {
        let po_path = po_dir.join(format!("{language}.po"));
        assert!(
            po_path.exists(),
            "translation generation error: PO file not found for language {language}: {}",
            po_path.display()
        );
        println!("cargo:rerun-if-changed={}", po_path.display());
        let lc_messages_dir = locale_dir.join(language).join("LC_MESSAGES");
        fs::create_dir_all(&lc_messages_dir).unwrap_or_else(|e| {
            panic!(
                "translation generation error: failed to create output directory {}: {e}",
                lc_messages_dir.display()
            )
        });
        let status = Command::new("msgmerge")
            .current_dir(&root_dir)
            .args(["--backup=off", "--update"])
            .arg(&po_path)
            .arg(po_dir.join(format!("{SHORT_NAME}.pot")))
            .status()
            .unwrap_or_else(|e| {
                panic!("translation generation error: failed to run msgmerge: {e}")
            });
        assert!(
            status.success(),
            "translation generation error: msgmerge failed with status {status}"
        );
        let status = Command::new("msgfmt")
            .current_dir(&root_dir)
            .arg(&po_path)
            .arg("--output-file")
            .arg(lc_messages_dir.join(format!("{SHORT_NAME}.mo")))
            .status()
            .unwrap_or_else(|e| panic!("translation generation error: failed to run msgfmt: {e}"));
        assert!(
            status.success(),
            "translation generation error: msgfmt failed with status {status}"
        );
    }
    tauri_build::build()
}

fn find_files(root_dir: &Path, dir: &Path, extensions: &[&str]) -> Vec<PathBuf> {
    let mut files = Vec::new();
    for entry in fs::read_dir(dir).unwrap_or_else(|e| {
        panic!(
            "translation generation error: failed to read directory {}: {e}",
            dir.display()
        )
    }) {
        let path = entry
            .unwrap_or_else(|e| {
                panic!(
                    "translation generation error: failed to read directory entry in {}: {e}",
                    dir.display()
                )
            })
            .path();
        if path.is_dir() {
            files.extend(find_files(root_dir, &path, extensions));
        } else if path
            .extension()
            .is_some_and(|ext| extensions.contains(&ext.to_str().unwrap_or_default()))
        {
            files.push(path.strip_prefix(root_dir).unwrap().to_path_buf());
        }
    }
    files
}
