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
    generate_translations();
}

fn read_list_file(path: &Path) -> Vec<String> {
    fs::read_to_string(path)
        .unwrap_or_else(|e| {
            panic!(
                "translation generation error: failed to read {}: {e}",
                path.display()
            )
        })
        .lines()
        .map(str::trim)
        .filter(|line| !line.is_empty() && !line.starts_with('#'))
        .map(ToOwned::to_owned)
        .collect()
}

fn run_command(command: &mut Command, program: &str) {
    let status = command
        .status()
        .unwrap_or_else(|e| panic!("translation generation error: failed to run {program}: {e}"));
    assert!(
        status.success(),
        "translation generation error: {program} failed with status {status}"
    );
}

fn generate_translations() {
    let root_dir =
        PathBuf::from(env::var("CARGO_MANIFEST_DIR").expect("CARGO_MANIFEST_DIR not set"))
            .ancestors()
            .find(|dir| dir.join("resources").join("po").join("POTFILES").exists())
            .expect("translation generation error: unable to locate root resources/po/POTFILES")
            .to_path_buf();
    let po_dir = root_dir.join("resources").join("po");
    let potfiles_path = po_dir.join("POTFILES");
    let linguas_path = po_dir.join("LINGUAS");
    let template_path = po_dir.join(format!("{SHORT_NAME}.pot"));
    let output_dir = PathBuf::from(env::var("OUT_DIR").expect("OUT_DIR not set"))
        .ancestors()
        .find(|dir| dir.file_name().is_some_and(|name| name == "build"))
        .expect("translation generation error: unable to locate Cargo build directory from OUT_DIR")
        .parent()
        .expect(
            "translation generation error: unable to locate profile output directory from OUT_DIR",
        )
        .to_path_buf();
    let sources = read_list_file(&potfiles_path);
    println!("cargo:rerun-if-changed={}", potfiles_path.display());
    println!("cargo:rerun-if-changed={}", linguas_path.display());
    for source in &sources {
        println!("cargo:rerun-if-changed={}", root_dir.join(source).display());
    }
    let mut xgettext_command = Command::new("xgettext");
    xgettext_command
        .current_dir(&root_dir)
        .arg("--from-code=utf-8")
        .arg("--language=Rust")
        .arg("--force-po")
        .arg(format!("--output={}", template_path.display()))
        .arg("--width=80");
    for keyword in XGETTEXT_KEYWORDS {
        xgettext_command.arg(format!("--keyword={keyword}"));
    }
    xgettext_command.args(&sources);
    run_command(&mut xgettext_command, "xgettext");
    for language in &read_list_file(&linguas_path) {
        let po_path = po_dir.join(format!("{language}.po"));
        assert!(
            po_path.exists(),
            "translation generation error: PO file not found for language {language}: {}",
            po_path.display()
        );
        println!("cargo:rerun-if-changed={}", po_path.display());
        let lc_messages_dir = output_dir.join(language).join("LC_MESSAGES");
        fs::create_dir_all(&lc_messages_dir).unwrap_or_else(|e| {
            panic!(
                "translation generation error: failed to create output directory {}: {e}",
                lc_messages_dir.display()
            )
        });
        run_command(
            Command::new("msgmerge")
                .current_dir(&root_dir)
                .args(["--backup=off", "--update"])
                .arg(&po_path)
                .arg(&template_path),
            "msgmerge",
        );
        run_command(
            Command::new("msgfmt")
                .current_dir(&root_dir)
                .arg(&po_path)
                .arg("--output-file")
                .arg(lc_messages_dir.join(format!("{SHORT_NAME}.mo"))),
            "msgfmt",
        );
    }
}
