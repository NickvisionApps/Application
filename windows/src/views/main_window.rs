use shared::{controller::AppController, translation};
use std::cell::RefCell;
use std::rc::Rc;
use windows_reactor::AcceleratorModifiers::Control;
use windows_reactor::*;

#[derive(Debug)]
pub struct MainWindow {
    controller: Rc<RefCell<AppController>>,
    navigation_pane_open: bool,
}

#[derive(Debug, Clone)]
pub enum MainWindowMessage {
    CloseFolder,
    NavigationPaneToggleRequested,
    OpenFolder,
}

impl Component for MainWindow {
    type Message = MainWindowMessage;
    type Input = ();

    fn create(_input: &(), _context: &ComponentContext<Self>) -> Self {
        MainWindow {
            controller: Rc::new(RefCell::new(AppController::default())),
            navigation_pane_open: true,
        }
    }

    fn update(&mut self, message: MainWindowMessage, _context: &ComponentContext<Self>) {
        match message {
            MainWindowMessage::CloseFolder => {
                self.controller.borrow_mut().folder_browser_mut().close();
            }
            MainWindowMessage::NavigationPaneToggleRequested => {
                self.navigation_pane_open = !self.navigation_pane_open;
            }
            MainWindowMessage::OpenFolder => {}
        }
    }

    fn view(&self, _input: &(), context: &mut ViewContext<Self>) -> View {
        let controller = self.controller.borrow();
        let geometry = controller.window_geometry();
        context.window_title(translation::_g("Application"));
        context.window_visuals(
            WindowVisuals::new()
                .backdrop(WindowBackdrop::Mica)
                .client_size(geometry.width() as f64, geometry.height() as f64),
        );
        Grid::new()
            .columns([GridLength::STAR])
            .rows([GridLength::Auto, GridLength::STAR])
            .children((
                TitleBar::new()
                    .grid_row(0)
                    .grid_column(0)
                    .preferred_height(WindowTitleBarHeight::Tall)
                    .height(48.0)
                    .title(translation::_g("Application"))
                    .is_back_button_visible(false)
                    .is_pane_toggle_button_visible(true)
                    .on_pane_toggle_requested(
                        context.message(MainWindowMessage::NavigationPaneToggleRequested),
                    ),
                NavigationView::new()
                    .grid_row(1)
                    .grid_column(0)
                    .horizontal_alignment(HorizontalAlignment::Stretch)
                    .vertical_alignment(VerticalAlignment::Stretch)
                    .pane_display_mode(NavigationViewPaneDisplayMode::Left)
                    .is_back_button_visible(NavigationViewBackButtonVisible::Collapsed)
                    .is_pane_toggle_button_visible(false)
                    .is_settings_visible(false)
                    .is_pane_open(self.navigation_pane_open),
            ))
    }
}
