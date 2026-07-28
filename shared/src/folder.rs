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
        self.path = path.into();
        self.files.clear();
        for entry in std::fs::read_dir(&self.path)? {
            let path = entry?.path();
            if path.is_file() {
                self.files.push(path);
            }
        }
        Ok(())
    }

    pub fn close(&mut self) {
        self.path = PathBuf::default()
    }

    pub fn files(&self) -> &[PathBuf] {
        &self.files
    }

    pub fn path(&self) -> &Path {
        self.path.as_path()
    }
}
