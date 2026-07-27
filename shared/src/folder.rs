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

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn check_directory() {
        #[cfg(target_os = "windows")]
        let path = "C:\\Windows\\System32";
        #[cfg(not(target_os = "windows"))]
        let path = "/usr/bin";
        let mut browser = FolderBrowser::default();
        let result = browser.open(path);
        assert!(result.is_ok());
        assert_eq!(browser.path(), path);
        assert!(browser.files().iter().count() > 0);
        browser.close();
    }
}
