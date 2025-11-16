using Microsoft.UI.Windowing;
using Microsoft.UI.Xaml;
using Microsoft.UI.Xaml.Controls;
using Nickvision.Application.Shared.Controllers;
using Nickvision.Application.Shared.Models;
using Nickvision.Desktop.Application;
using Vanara.PInvoke;
using Windows.Graphics;
using WinRT.Interop;

namespace Nickvision.Application.WinUI.Views;

public sealed partial class MainWindow : Window
{
    private readonly MainWindowController _controller;
    private readonly nint _hwnd;

    public MainWindow(MainWindowController controller)
    {
        InitializeComponent();
        _controller = controller;
        _hwnd = WindowNative.GetWindowHandle(this);
        // Theme
        MainGrid.RequestedTheme = _controller.Theme switch
        {
            Theme.Light => ElementTheme.Light,
            Theme.Dark => ElementTheme.Dark,
            _ => ElementTheme.Default
        };
        // Size
        if (_controller.WindowGeometry.IsMaximized)
        {
            AppWindow.Resize(new SizeInt32
            {
                Width = 900,
                Height = 700
            });
            User32.ShowWindow(_hwnd, ShowWindowCommand.SW_SHOWMAXIMIZED);
        }
        else
        {
            AppWindow.MoveAndResize(new RectInt32
            {
                X = _controller.WindowGeometry.X,
                Y = _controller.WindowGeometry.Y,
                Width = _controller.WindowGeometry.Width,
                Height = _controller.WindowGeometry.Height
            });
        }
        // TitleBar
        AppWindow.SetIcon("./Assets/org.nickvision.application.ico");
        ExtendsContentIntoTitleBar = true;
        SetTitleBar(TitleBar);
        AppWindow.TitleBar.PreferredHeightOption = TitleBarHeightOption.Tall;
        // Events
        AppWindow.Closing += Window_Closing;
        // Translations
        AppWindow.Title = _controller.AppInfo.ShortName;
        TitleBar.Title = _controller.AppInfo.ShortName;
        TitleBar.Subtitle = _controller.AppInfo.Version!.IsPreview ? _controller.Translator._("Preview") : string.Empty;
        MenuFile.Title = _controller.Translator._("File");
        MenuOpenFolder.Text = _controller.Translator._("Open Folder");
        MenuCloseFolder.Text = _controller.Translator._("Close Folder");
        MenuExit.Text = _controller.Translator._("Exit");
        MenuEdit.Title = _controller.Translator._("Edit");
        MenuSettings.Text = _controller.Translator._("Settings");
        MenuHelp.Title = _controller.Translator._("Help");
        NavItemHome.Content = _controller.Translator._("Home");
        NavItemSettings.Content = _controller.Translator._("Settings");
        StatusHome.Title = _controller.Greeting;
        StatusHome.Description = _controller.Translator._("Open a folder to get started");
        LblHomeOpenFolder.Text = _controller.Translator._("Open Folder");
    }

    private void Window_Closing(AppWindow sender, AppWindowClosingEventArgs args)
    {
        if (!_controller.CanShutdown)
        {
            args.Cancel = true;
            return;
        }
        _controller.WindowGeometry = new WindowGeometry(AppWindow.Size.Width, AppWindow.Size.Height, User32.IsZoomed(_hwnd), AppWindow.Position.X, AppWindow.Position.Y);
    }

    private void TitleBar_PaneToggleRequested(TitleBar sender, object args)
    {
        NavView.IsPaneOpen = !NavView.IsPaneOpen;
    }

    private void NavView_SelectionChanged(NavigationView sender, NavigationViewSelectionChangedEventArgs args)
    {
        if (args.SelectedItem is NavigationViewItem item)
        {
            ViewStack.SelectedIndex = (item.Tag as string) switch
            {
                "Settings" => 1,
                _ => 0
            };
        }
    }

    private void OpenFolder(object sender, RoutedEventArgs e)
    {

    }

    private void CloseFolder(object sender, RoutedEventArgs e)
    {

    }

    private void Exit(object sender, RoutedEventArgs e) => Close();

    private void Settings(object sender, RoutedEventArgs e) => NavItemSettings.IsSelected = true;
}
