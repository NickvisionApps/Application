using Microsoft.UI;
using Microsoft.UI.Windowing;
using Microsoft.UI.Xaml;
using Microsoft.UI.Xaml.Controls;
using Microsoft.UI.Xaml.Media;
using NickvisionApplication.Shared.Controllers;
using NickvisionApplication.Shared.Events;
using NickvisionApplication.WinUI.Controls;
using System;
using System.IO;
using Vanara.PInvoke;
using Windows.ApplicationModel.DataTransfer;
using Windows.Graphics;
using Windows.Storage;
using Windows.Storage.Pickers;
using Windows.System;
using WinRT.Interop;

namespace NickvisionApplication.WinUI.Views;

/// <summary>
/// The MainWindow for the application
/// </summary>
public sealed partial class MainWindow : Window
{
    private readonly MainWindowController _controller;
    private readonly IntPtr _hwnd;
    private bool _isActived;

    /// <summary>
    /// Constructs a MainWindow
    /// </summary>
    /// <param name="controller">The MainWindowController</param>
    public MainWindow(MainWindowController controller)
    {
        InitializeComponent();
        //Initialize Vars
        _controller = controller;
        _hwnd = WindowNative.GetWindowHandle(this);
        _isActived = true;
        //Register Events
        AppWindow.Closing += Window_Closing;
        _controller.NotificationSent += NotificationSent;
        _controller.FolderChanged += FolderChanged;
        //Set TitleBar
        TitleBarTitle.Text = _controller.AppInfo.ShortName;
        AppWindow.Title = TitleBarTitle.Text;
        AppWindow.SetIcon(@"Assets\org.nickvision.application.ico");
        TitlePreview.Text = _controller.IsDevVersion ? _controller.Localizer["Preview", "WinUI"] : "";
        if (AppWindowTitleBar.IsCustomizationSupported())
        {
            AppWindow.TitleBar.ExtendsContentIntoTitleBar = true;
            TitleBarLeftPaddingColumn.Width = new GridLength(AppWindow.TitleBar.LeftInset);
            TitleBarRightPaddingColumn.Width = new GridLength(AppWindow.TitleBar.RightInset);
            AppWindow.TitleBar.ButtonBackgroundColor = Colors.Transparent;
            AppWindow.TitleBar.ButtonInactiveBackgroundColor = Colors.Transparent;
        }
        else
        {
            TitleBar.Visibility = Visibility.Collapsed;
        }
        SystemBackdrop = new MicaBackdrop();
        //Window Sizing
        AppWindow.Resize(new SizeInt32(800, 600));
        User32.ShowWindow(_hwnd, ShowWindowCommand.SW_SHOWMAXIMIZED);
        //Localize Strings
        MenuFile.Title = _controller.Localizer["File"];
        MenuOpenFolder.Text = _controller.Localizer["OpenFolder"];
        MenuCloseFolder.Text = _controller.Localizer["CloseFolder"];
        MenuExit.Text = _controller.Localizer["Exit"];
        MenuEdit.Title = _controller.Localizer["Edit"];
        MenuSettings.Text = _controller.Localizer["Settings"];
        MenuChangelog.Text = _controller.Localizer["Changelog"];
        MenuGitHubRepo.Text = _controller.Localizer["GitHubRepo"];
        MenuReportABug.Text = _controller.Localizer["ReportABug"];
        MenuDiscussions.Text = _controller.Localizer["Discussions"];
        MenuAbout.Text = string.Format(_controller.Localizer["About"], _controller.AppInfo.ShortName);
        MenuHelp.Title = _controller.Localizer["Help"];
        LblStatus.Text = _controller.Localizer["StatusReady", "WinUI"];
        ToolTipService.SetToolTip(BtnOpenNotifications, _controller.Localizer["OpenNotifications", "WinUI"]);
        LblNotifications.Text = _controller.Localizer["Notifications", "WinUI"];
        ToolTipService.SetToolTip(BtnClearNotifications, _controller.Localizer["ClearNotifications", "WinUI"]);
        StatusPageHome.Glyph = _controller.ShowSun ? "\xE706" : "\xE708";
        StatusPageHome.Title = _controller.Greeting;
        StatusPageHome.Description = _controller.Localizer["NoFolderDescription"];
        ToolTipService.SetToolTip(BtnHomeOpenFolder, _controller.Localizer["OpenFolder", "Tooltip"]);
        LblBtnHomeOpenFolder.Text = _controller.Localizer["Open"];
        BtnCloseFolder.Label = _controller.Localizer["CloseFolder"];
        ToolTipService.SetToolTip(BtnCloseFolder, _controller.Localizer["CloseFolder", "Tooltip"]);
        //Pages
        ViewStack.ChangePage("Home");
    }

    /// <summary>
    /// Calls InitializeWithWindow.Initialize on the target object with the MainWindow's hwnd
    /// </summary>
    /// <param name="target">The target object to initialize</param>
    public void InitializeWithWindow(object target) => WinRT.Interop.InitializeWithWindow.Initialize(target, _hwnd);

    /// <summary>
    /// Occurs when the window is activated
    /// </summary>
    /// <param name="sender">object</param>
    /// <param name="e">WindowActivatedEventArgs</param>
    private void Window_Activated(object sender, WindowActivatedEventArgs e)
    {
        _isActived = e.WindowActivationState != WindowActivationState.Deactivated;
        //Update TitleBar
        TitleBarTitle.Foreground = (SolidColorBrush)Application.Current.Resources[_isActived ? "WindowCaptionForeground" : "WindowCaptionForegroundDisabled"];
        AppWindow.TitleBar.ButtonForegroundColor = ((SolidColorBrush)Application.Current.Resources[_isActived ? "WindowCaptionForeground" : "WindowCaptionForegroundDisabled"]).Color;
    }

    /// <summary>
    /// Occurs when the window is closing
    /// </summary>
    /// <param name="sender">AppWindow</param>
    /// <param name="e">AppWindowClosingEventArgs</param>
    private void Window_Closing(AppWindow sender, AppWindowClosingEventArgs e)
    {
        _controller.Dispose();
    }

    /// <summary>
    /// Occurs when the window's theme is changed
    /// </summary>
    /// <param name="sender">FrameworkElement</param>
    /// <param name="e">object</param>
    private void Window_ActualThemeChanged(FrameworkElement sender, object e)
    {
        //Update TitleBar
        TitleBarTitle.Foreground = (SolidColorBrush)Application.Current.Resources[_isActived ? "WindowCaptionForeground" : "WindowCaptionForegroundDisabled"];
        AppWindow.TitleBar.ButtonForegroundColor = ((SolidColorBrush)Application.Current.Resources[_isActived ? "WindowCaptionForeground" : "WindowCaptionForegroundDisabled"]).Color;
    }

    /// <summary>
    /// Occurs when something is dragged over the window
    /// </summary>
    /// <param name="sender">object</param>
    /// <param name="e">DragEventArgs</param>
    private void Window_DragOver(object sender, DragEventArgs e) => e.AcceptedOperation = DataPackageOperation.Link;

    /// <summary>
    /// Occurs when something is dropped on the window
    /// </summary>
    /// <param name="sender">object</param>
    /// <param name="e">DragEventArgs</param>
    private async void Window_Drop(object sender, DragEventArgs e)
    {
        if (e.DataView.Contains(StandardDataFormats.StorageItems))
        {
            var items = await e.DataView.GetStorageItemsAsync();
            if (items.Count > 0)
            {
                foreach (var item in items)
                {
                    if (item is StorageFolder folder)
                    {
                        _controller.OpenFolder(folder.Path);
                        break;
                    }
                }
            }
        }
    }

    /// <summary>
    /// Occurs when a notification is sent from the controller
    /// </summary>
    /// <param name="sender">object?</param>
    /// <param name="e">NotificationSentEventArgs</param>
    private void NotificationSent(object? sender, NotificationSentEventArgs e)
    {
        //InfoBar
        InfoBar.Message = e.Message;
        InfoBar.Severity = e.Severity switch
        {
            NotificationSeverity.Informational => InfoBarSeverity.Informational,
            NotificationSeverity.Success => InfoBarSeverity.Success,
            NotificationSeverity.Warning => InfoBarSeverity.Warning,
            NotificationSeverity.Error => InfoBarSeverity.Error,
            _ => InfoBarSeverity.Informational
        };
        InfoBar.IsOpen = true;
        //Notification Badge
        BadgeNotification.Value++;
        BadgeNotification.Style = (Style)Application.Current.Resources["AttentionValueInfoBadgeStyle"];
        //Notifiation Flyout
        var newNotification = new Border()
        {
            Margin = new Thickness(0, 0, 0, 6),
            Background = (Brush)Application.Current.Resources["CardBackgroundFillColorDefaultBrush"],
            BorderBrush = (Brush)Application.Current.Resources["CardStrokeColorDefaultBrush"],
            BorderThickness = new Thickness(1, 1, 1, 1),
            CornerRadius = new CornerRadius(6.0)
        };
        var newNotificationStackPanel = new StackPanel()
        {
            Margin = new Thickness(12, 12, 12, 12),
            Orientation = Orientation.Horizontal,
            Spacing = 6
        };
        newNotificationStackPanel.Children.Add(new FontIcon()
        {
            FontFamily = (Microsoft.UI.Xaml.Media.FontFamily)Application.Current.Resources["SymbolThemeFontFamily"],
            Glyph = e.Severity switch
            {
                NotificationSeverity.Success => "\uE73E",
                NotificationSeverity.Warning => "\uE7BA",
                NotificationSeverity.Error => "\uEA39",
                _ => "\uE946"
            }
        });
        newNotificationStackPanel.Children.Add(new TextBlock()
        {
            Text = e.Message,
            TextWrapping = TextWrapping.Wrap
        });
        newNotification.Child = newNotificationStackPanel;
        ListNotifications.Items.Insert(0, newNotification);
    }

    /// <summary>
    /// Occurs when the open notifications button is clicked
    /// </summary>
    /// <param name="sender">object</param>
    /// <param name="e">RoutedEventArgs</param>
    private void OpenNotifications(object sender, RoutedEventArgs e) => InfoBar.IsOpen = false;

    /// <summary>
    /// Occurs when the clear notifications button is clicked
    /// </summary>
    /// <param name="sender">object</param>
    /// <param name="e">RoutedEventArgs</param>
    private void ClearNotifications(object sender, RoutedEventArgs e)
    {
        ListNotifications.Items.Clear();
        FlyoutNotifications.Hide();
        BadgeNotification.Value = 0;
        BadgeNotification.Style = (Style)Application.Current.Resources["InformationalValueInfoBadgeStyle"];
    }

    /// <summary>
    /// Occurs when the folder is changed
    /// </summary>
    /// <param name="sender">object?</param>
    /// <param name="e">EventArgs</param>
    private void FolderChanged(object? sender, EventArgs e)
    {
        if(Directory.Exists(_controller.FolderPath))
        {
            IconStatus.Glyph = "\uE8B7";
            ViewStack.ChangePage("Folder");
            MenuCloseFolder.IsEnabled = true;
        }
        else
        {
            IconStatus.Glyph = "\uE73E";
            ViewStack.ChangePage("Home");
            MenuCloseFolder.IsEnabled = false;
        }
        LblStatus.Text = _controller.FolderPath;
    }

    /// <summary>
    /// Occurs when the open menu item is clicked
    /// </summary>
    /// <param name="sender">object</param>
    /// <param name="e">RoutedEventArgs</param>
    private async void OpenFolder(object sender, RoutedEventArgs e)
    {
        var folderPicker = new FolderPicker();
        InitializeWithWindow(folderPicker);
        folderPicker.FileTypeFilter.Add("*");
        var file = await folderPicker.PickSingleFolderAsync();
        if (file != null)
        {
            _controller.OpenFolder(file.Path);
        }
    }

    /// <summary>
    /// Occurs when the close folder menu item is clicked
    /// </summary>
    /// <param name="sender">object</param>
    /// <param name="e">RoutedEventArgs</param>
    private void CloseFolder(object sender, RoutedEventArgs e) => _controller.CloseFolder();

    /// <summary>
    /// Occurs when the exit menu item is clicked
    /// </summary>
    /// <param name="sender">object</param>
    /// <param name="e">RoutedEventArgs</param>
    private void Exit(object sender, RoutedEventArgs e) => Close();

    /// <summary>
    /// Occurs when the settings menu item is clicked
    /// </summary>
    /// <param name="sender">object</param>
    /// <param name="e">RoutedEventArgs</param>
    private async void Settings(object sender, RoutedEventArgs e)
    {
        var preferencesDialog = new SettingsDialog(_controller.CreatePreferencesViewController())
        {
            XamlRoot = Content.XamlRoot
        };
        await preferencesDialog.ShowAsync();
    }

    /// <summary>
    /// Occurs when the changelog menu item is clicked
    /// </summary>
    /// <param name="sender">object</param>
    /// <param name="e">RoutedEventArgs</param>
    private async void Changelog(object sender, RoutedEventArgs e)
    {
        var changelogDialog = new ContentDialog()
        {
            Title = _controller.Localizer["ChangelogTitle", "WinUI"],
            Content = _controller.AppInfo.Changelog,
            CloseButtonText = _controller.Localizer["OK"],
            DefaultButton = ContentDialogButton.Close,
            XamlRoot = Content.XamlRoot
        };
        await changelogDialog.ShowAsync();
    }

    /// <summary>
    /// Occurs when the github repo menu item is clicked
    /// </summary>
    /// <param name="sender">object</param>
    /// <param name="e">RoutedEventArgs</param>
    private async void GitHubRepo(object sender, RoutedEventArgs e) => await Launcher.LaunchUriAsync(_controller.AppInfo.GitHubRepo);

    /// <summary>
    /// Occurs when the report a bug menu item is clicked
    /// </summary>
    /// <param name="sender">object</param>
    /// <param name="e">RoutedEventArgs</param>
    private async void ReportABug(object sender, RoutedEventArgs e) => await Launcher.LaunchUriAsync(_controller.AppInfo.IssueTracker);

    /// <summary>
    /// Occurs when the discussions menu item is clicked
    /// </summary>
    /// <param name="sender">object</param>
    /// <param name="e">RoutedEventArgs</param>
    private async void Discussions(object sender, RoutedEventArgs e) => await Launcher.LaunchUriAsync(_controller.AppInfo.SupportUrl);

    /// <summary>
    /// Occurs when the about menu item is clicked
    /// </summary>
    /// <param name="sender">object</param>
    /// <param name="e">RoutedEventArgs</param>
    private async void About(object sender, RoutedEventArgs e)
    {
        var changelogDialog = new ContentDialog()
        {
            Title = _controller.Localizer["ChangelogTitle", "WinUI"],
            Content = _controller.AppInfo.Changelog,
            CloseButtonText = _controller.Localizer["OK"],
            DefaultButton = ContentDialogButton.Close,
            XamlRoot = Content.XamlRoot
        };
        await changelogDialog.ShowAsync();
    }
}
