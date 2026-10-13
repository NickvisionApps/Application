#[derive(Debug, Clone, Copy, Default)]
pub struct CloseManager {
    can_close: bool,
}

impl CloseManager {
    pub fn can_close(&self) -> bool {
        self.can_close
    }

    pub fn confirm_close(&mut self) {
        self.can_close = true;
    }
}
