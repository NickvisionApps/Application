#[derive(Debug, Clone, Copy, Default)]
pub struct CloseManager {
    can_close: bool,
}

impl CloseManager {
    pub fn new() -> Self {
        CloseManager::default()
    }

    pub fn can_close(&self) -> bool {
        self.can_close
    }

    pub fn confirm_close(&mut self) {
        self.can_close = true;
    }

    pub fn set_can_close(&mut self, can_close: bool) {
        self.can_close = can_close;
    }
}
