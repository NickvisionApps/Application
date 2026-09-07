use crate::helpers::InfoBarMessage;
use shared::{
    config::{ApplicationTheme, WindowGeometry},
    controller::AppController,
    translation,
};
use std::cell::RefCell;
use std::rc::Rc;
use windows::{
    Storage::Pickers::{FolderPicker, PickerLocationId, PickerViewMode},
    Win32::UI::{
        Shell::IInitializeWithWindow,
        WindowsAndMessaging::{GetForegroundWindow, IsZoomed},
    },
    core::{Interface, h},
};
use windows_reactor::*;

#[derive(Debug)]
pub struct MainWindow {
    controller: Rc<RefCell<AppController>>,
    default_geometry: WindowGeometry,
    info_bar_message: Option<InfoBarMessage>,
    navigation_pane_open: bool,
    navigation_selected_tag: String,
    navigation_tag_history: Vec<String>,
}

#[derive(Debug, Clone)]
pub enum MainWindowMessage {
    CloseFolder,
    InfoBarClosed,
    NavigationBackRequested,
    NavigationPaneToggleRequested,
    NavigationSelectedTagChanged(Option<String>),
    OpenFolder,
    WindowSizeChanged(WindowSize),
}

impl Component for MainWindow {
    type Message = MainWindowMessage;
    type Input = ();

    fn create(_input: &(), _context: &ComponentContext<Self>) -> Self {
        let controller = Rc::new(RefCell::new(AppController::default()));
        let geometry = controller.borrow().window_geometry().clone();
        MainWindow {
            controller,
            default_geometry: geometry,
            info_bar_message: None,
            navigation_pane_open: true,
            navigation_selected_tag: "Home".to_string(),
            navigation_tag_history: Vec::new(),
        }
    }

    fn update(&mut self, message: MainWindowMessage, _context: &ComponentContext<Self>) {
        match message {
            MainWindowMessage::CloseFolder => {
                self.controller.borrow_mut().folder_browser_mut().close();
                self.info_bar_message = Some(InfoBarMessage::new(
                    translation::_g("Folder closed"),
                    "",
                    InfoBarSeverity::Warning,
                ));
            }
            MainWindowMessage::InfoBarClosed => {
                self.info_bar_message = None;
            }
            MainWindowMessage::NavigationBackRequested => {
                if let Some(previous) = self.navigation_tag_history.pop() {
                    self.navigation_selected_tag = previous;
                }
            }
            MainWindowMessage::NavigationPaneToggleRequested => {
                self.navigation_pane_open = !self.navigation_pane_open;
            }
            MainWindowMessage::NavigationSelectedTagChanged(tag) => {
                if let Some(tag) = tag {
                    self.navigation_tag_history
                        .push(self.navigation_selected_tag.clone());
                    self.navigation_selected_tag = tag;
                }
            }
            MainWindowMessage::OpenFolder => {
                let picker = FolderPicker::new().unwrap();
                picker.FileTypeFilter().unwrap().Append(h!("*")).unwrap();
                picker
                    .SetSuggestedStartLocation(PickerLocationId::DocumentsLibrary)
                    .unwrap();
                picker.SetViewMode(PickerViewMode::List).unwrap();
                let initalizer = picker.cast::<IInitializeWithWindow>().unwrap();
                unsafe { initalizer.Initialize(GetForegroundWindow()) }.unwrap();
                let result = picker.PickSingleFolderAsync().unwrap().GetResults();
                if let Ok(folder) = result
                    && let Err(error) = self
                        .controller
                        .borrow_mut()
                        .folder_browser_mut()
                        .open(folder.Path().unwrap().to_string_lossy())
                {
                    self.info_bar_message = Some(InfoBarMessage::new(
                        translation::_g("Error"),
                        translation::_f("Unable to open folder: {0}", &[error.to_string()]),
                        InfoBarSeverity::Error,
                    ));
                }
            }
            MainWindowMessage::WindowSizeChanged(size) => {
                let mut controller = self.controller.borrow_mut();
                controller.set_window_geometry(
                    if unsafe { IsZoomed(GetForegroundWindow()).as_bool() } {
                        WindowGeometry::builder().is_maximized(true).build()
                    } else {
                        WindowGeometry::builder()
                            .width(size.width as u64)
                            .height(size.height as u64)
                            .build()
                    },
                );
                controller.save().unwrap();
            }
        }
    }

    fn view(&self, _input: &(), context: &mut ViewContext<Self>) -> View {
        let controller = self.controller.borrow();
        context.window_title(translation::_g("Application"));
        context.window_visuals(
            WindowVisuals::new()
                .backdrop(WindowBackdrop::Mica)
                .client_size(
                    self.default_geometry.width() as f64,
                    self.default_geometry.height() as f64,
                )
                .theme(match controller.theme() {
                    ApplicationTheme::System => WindowTheme::System,
                    ApplicationTheme::Light => WindowTheme::Light,
                    ApplicationTheme::Dark => WindowTheme::Dark,
                })
                .constraints(WindowConstraints {
                    min_width: Some(800.0),
                    min_height: Some(600.0),
                    max_width: None,
                    max_height: None,
                }),
        );
        context.on_window_size(context.callback(MainWindowMessage::WindowSizeChanged));
        Grid::new()
            .columns([GridLength::STAR])
            .rows([GridLength::Auto, GridLength::Auto, GridLength::STAR])
            .children((
                TitleBar::new()
                    .grid_row(0)
                    .grid_column(0)
                    .preferred_height(WindowTitleBarHeight::Tall)
                    .title(translation::_g("Application"))
                    .is_back_button_visible(true)
                    .is_back_button_enabled(!self.navigation_tag_history.is_empty())
                    .is_pane_toggle_button_visible(true)
                    .on_back_requested(context.message(MainWindowMessage::NavigationBackRequested))
                    .on_pane_toggle_requested(
                        context.message(MainWindowMessage::NavigationPaneToggleRequested),
                    )
                    .slots([SlotView::new(TitleBarSlot::Content, TextBlock::new())]),
                InfoBar::new()
                    .grid_row(1)
                    .grid_column(0)
                    .is_open(self.info_bar_message.is_some())
                    .message(if let Some(info_bar_message) = &self.info_bar_message {
                        info_bar_message.message()
                    } else {
                        ""
                    })
                    .title(if let Some(info_bar_message) = &self.info_bar_message {
                        info_bar_message.title()
                    } else {
                        ""
                    })
                    .severity(if let Some(info_bar_message) = &self.info_bar_message {
                        info_bar_message.severity()
                    } else {
                        InfoBarSeverity::Informational
                    })
                    .on_closed(context.message(MainWindowMessage::InfoBarClosed)),
                NavigationView::new()
                    .grid_row(2)
                    .grid_column(0)
                    .horizontal_alignment(HorizontalAlignment::Stretch)
                    .vertical_alignment(VerticalAlignment::Stretch)
                    .pane_display_mode(NavigationViewPaneDisplayMode::Left)
                    .is_back_button_visible(NavigationViewBackButtonVisible::Collapsed)
                    .is_pane_toggle_button_visible(false)
                    .is_settings_visible(true)
                    .is_pane_open(self.navigation_pane_open)
                    .on_selected_tag_changed(
                        context.callback(MainWindowMessage::NavigationSelectedTagChanged),
                    )
                    .slots([
                        SlotView::collection(
                            NavigationViewSlot::MenuItems,
                            [
                                KeyedView::new(
                                    "Home",
                                    NavigationViewItem::new()
                                        .tag("Home")
                                        .is_selected(self.navigation_selected_tag == "Home")
                                        .slots([
                                            SlotView::new(
                                                NavigationViewItemSlot::Content,
                                                translation::_g("Home"),
                                            ),
                                            SlotView::new(
                                                NavigationViewItemSlot::Icon,
                                                SymbolIcon::new().symbol(Symbol::Home),
                                            ),
                                        ]),
                                ),
                                KeyedView::new(
                                    "Folder",
                                    NavigationViewItem::new()
                                        .tag("Folder")
                                        .is_selected(self.navigation_selected_tag == "Folder")
                                        .slots([
                                            SlotView::new(
                                                NavigationViewItemSlot::Content,
                                                translation::_g("Folder"),
                                            ),
                                            SlotView::new(
                                                NavigationViewItemSlot::Icon,
                                                SymbolIcon::new().symbol(Symbol::Folder),
                                            ),
                                        ]),
                                ),
                            ],
                        ),
                        SlotView::new(
                            NavigationViewSlot::Content,
                            if self.navigation_selected_tag == "Home" {
                                TextBlock::new().text(translation::_g("Home"))
                            } else if self.navigation_selected_tag == "Folder" {
                                TextBlock::new().text(translation::_g("Folder"))
                            } else if self.navigation_selected_tag == "Settings" {
                                TextBlock::new().text(translation::_g("Settings"))
                            } else {
                                TextBlock::new().text("")
                            },
                        ),
                    ]),
            ))
    }
}
