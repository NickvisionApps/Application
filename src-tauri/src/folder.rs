use serde::{Deserialize, Serialize};
use std::path::{Path, PathBuf};

#[derive(Debug, Clone, Default, PartialEq, Eq, Serialize, Deserialize)]
#[serde(rename_all = "camelCase")]
pub struct FolderView {
    path: PathBuf,
    files: Vec<PathBuf>,
}

impl FolderView {
    pub fn new(path: impl Into<PathBuf>) -> Result<Self, std::io::Error> {
        let mut files = Vec::new();
        let path = path.into();
        for entry in std::fs::read_dir(&path)? {
            let path = entry?.path();
            if path.is_file() && !path.is_symlink() {
                files.push(path);
            }
        }
        files.sort();
        Ok(Self { path, files })
    }

    pub fn files(&self) -> &[PathBuf] {
        &self.files
    }

    pub fn path(&self) -> &Path {
        &self.path
    }
}
