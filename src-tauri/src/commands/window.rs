use crate::close::CloseManager;
use std::sync::Mutex;
use tauri::{State, command};

#[command]
pub fn can_window_close(close_manager: State<'_, Mutex<CloseManager>>) -> bool {
    close_manager.lock().unwrap().can_close()
}

#[command]
pub fn confirm_window_close(close_manager: State<'_, Mutex<CloseManager>>) {
    close_manager.lock().unwrap().confirm_close();
}
