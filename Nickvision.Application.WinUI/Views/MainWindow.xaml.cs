using Microsoft.UI.Windowing;
using Microsoft.UI.Xaml;
using Microsoft.UI.Xaml.Controls;
using Microsoft.UI.Xaml.Input;
using Microsoft.Windows.Storage.Pickers;
using Nickvision.Application.Shared.Controllers;
using Nickvision.Application.Shared.Events;
using Nickvision.Application.Shared.Models;
using Nickvision.Desktop.Application;
using Nickvision.Desktop.Notifications;
using System;
using System.IO;
using Vanara.PInvoke;
using Windows.Graphics;
using Windows.Storage;
using Windows.System;
using WinRT.Interop;

namespace Nickvision.Application.WinUI.Views;

public sealed partial class MainWindow : Window
{
    private enum Pages
    {
        Home = 0,
        Folder = 1,
        Settings = 2
    }

    private readonly MainWindowController _controller;
    private readonly nint _hwnd;
    private RoutedEventHandler? _notificationClickHandler;

    public MainWindow(MainWindowController controller)
    {
        InitializeComponent();
        _controller = controller;
        _hwnd = WindowNative.GetWindowHandle(this);
        _notificationClickHandler = null;
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
        _controller.AppNotificationSent += (sender, args) => DispatcherQueue.TryEnqueue(() => Controller_AppNotificationSent(sender, args));
        _controller.FolderChanged += Controller_FolderChanged;
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
        NavItemFolder.Content = _controller.Translator._("Folder");
        NavItemSettings.Content = _controller.Translator._("Settings");
        StatusHome.Title = _controller.Greeting;
        StatusHome.Description = _controller.Translator._("Open a folder to get started");
        LblHomeOpenFolder.Text = _controller.Translator._("Open Folder");
        BtnFolderOpenFolder.Label = _controller.Translator._("Open");
        BtnFolderCloseFolder.Label = _controller.Translator._("Close");
    }

    private void Window_Closing(AppWindow sender, AppWindowClosingEventArgs args)
    {
        if (!_controller.CanShutdown)
        {
            args.Cancel = true;
            return;
        }
        _controller.WindowGeometry = new WindowGeometry(AppWindow.Size.Width, AppWindow.Size.Height, User32.IsZoomed(_hwnd), AppWindow.Position.X, AppWindow.Position.Y);
        _controller.Dispose();
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
                "Folder" => (int)Pages.Folder,
                "Settings" => (int)Pages.Settings,
                _ => (int)Pages.Home
            };
        }
    }

    private void Controller_AppNotificationSent(object? sender, AppNotificationSentEventArgs args)
    {
        if (_notificationClickHandler is not null)
        {
            BtnInfoBar.Click -= _notificationClickHandler;
            _notificationClickHandler = null;
        }
        InfoBar.Message = args.Notification.Message;
        InfoBar.Severity = args.Notification.Severity switch
        {
            NotificationSeverity.Success => InfoBarSeverity.Success,
            NotificationSeverity.Warning => InfoBarSeverity.Warning,
            NotificationSeverity.Error => InfoBarSeverity.Error,
            _ => InfoBarSeverity.Informational
        };
        if (args.Notification.Action == "close")
        {
            BtnInfoBar.Content = _controller.Translator._("Close");
            _notificationClickHandler = CloseFolder;
            BtnInfoBar.Click += _notificationClickHandler;
        }
        BtnInfoBar.Visibility = _notificationClickHandler is not null ? Visibility.Visible : Visibility.Collapsed;
        InfoBar.IsOpen = true;
    }

    private void Controller_FolderChanged(object? sender, FolderChangedEventArgs args)
    {
        foreach (var item in ListFolderFiles.Items)
        {
            if (item is ListViewItem listViewItem)
            {
                listViewItem.DoubleTapped -= ListFolderFiles_ItemDoubleTapped;
            }
        }
        ListFolderFiles.Items.Clear();
        if (args.IsOpen)
        {
            NavItemFolder.Visibility = Visibility.Visible;
            NavItemFolder.IsSelected = true;
            InfoBadgeFolder.Value = args.Files.Count;
            LblFolderPath.Text = args.Path;
            foreach (var file in args.Files)
            {
                var item = new ListViewItem();
                item.Content = Path.GetFileName(file);
                item.Tag = file;
                item.DoubleTapped += ListFolderFiles_ItemDoubleTapped;
                ListFolderFiles.Items.Add(item);
            }
        }
        else
        {
            NavItemFolder.Visibility = Visibility.Collapsed;
            NavItemHome.IsSelected = true;
        }
    }


    private async void ListFolderFiles_ItemDoubleTapped(object sender, DoubleTappedRoutedEventArgs args)
    {
        if (sender is ListViewItem item)
        {
            var tag = item.Tag as string;
            if (string.IsNullOrEmpty(tag))
            {
                return;
            }
            await Launcher.LaunchFileAsync(await StorageFile.GetFileFromPathAsync(tag));
        }
    }

    private async void OpenFolder(object sender, RoutedEventArgs e)
    {
        var picker = new FolderPicker(AppWindow.Id);
        var result = await picker.PickSingleFolderAsync();
        if (result is null)
        {
            return;
        }
        _controller.OpenFolder(result.Path);
    }

    private void CloseFolder(object sender, RoutedEventArgs e) => _controller.CloseFolder();

    private void Exit(object sender, RoutedEventArgs e) => Close();

    private void Settings(object sender, RoutedEventArgs e) => NavItemSettings.IsSelected = true;
}
