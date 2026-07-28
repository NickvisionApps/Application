use std::path::{Path, PathBuf};

#[derive(Debug, Clone, Default)]
pub struct FolderBrowser {
    path: PathBuf,
    files: Vec<PathBuf>,
}

impl FolderBrowser {
    pub fn new() -> Self {
        FolderBrowser::default()
    }

    pub fn open(&mut self, path: impl Into<PathBuf>) -> Result<(), std::io::Error> {
        let mut files = Vec::new();
        let path = path.into();
        for entry in std::fs::read_dir(&path)? {
            let path = entry?.path();
            if path.is_file() {
                files.push(path);
            }
        }
        self.path = path;
        self.files = files;
        Ok(())
    }

    pub fn close(&mut self) {
        *self = Self::default();
    }

    pub fn files(&self) -> &[PathBuf] {
        &self.files
    }

    pub fn path(&self) -> &Path {
        self.path.as_path()
    }
}
