mod views;

use crate::views::MainWindow;
use windows_reactor::App;

pub fn run() {
    App::run_component::<MainWindow>(()).unwrap();
}
