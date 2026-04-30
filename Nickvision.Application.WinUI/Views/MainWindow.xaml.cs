using Microsoft.Extensions.DependencyInjection;
using Microsoft.Extensions.Hosting;
using Microsoft.UI.Windowing;
using Microsoft.UI.Xaml;
using Microsoft.UI.Xaml.Controls;
using Microsoft.Windows.Storage.Pickers;
using Nickvision.Application.Shared.Controllers;
using Nickvision.Application.Shared.Events;
using Nickvision.Application.Shared.Models;
using Nickvision.Application.Shared.Services;
using Nickvision.Application.WinUI.Controls;
using Nickvision.Desktop.Application;
using Nickvision.Desktop.Globalization;
using Nickvision.Desktop.Network;
using Nickvision.Desktop.Notifications;
using Nickvision.Desktop.WinUI.Controls;
using Nickvision.Desktop.WinUI.Helpers;
using System;
using System.IO;
using Windows.System;

namespace Nickvision.Application.WinUI.Views;

public sealed partial class MainWindow : Window
{
    private enum Pages
    {
        Home = 0,
        Folder = 1,
        Custom = 2
    }

    private readonly IServiceProvider _serviceProvider;
    private readonly MainWindowController _controller;
    private readonly AppInfo _appInfo;
    private readonly ITranslationService _translationService;
    private RoutedEventHandler? _notificationClickHandler;

    public MainWindow(IServiceProvider serviceProvider, MainWindowController controller, AppInfo appInfo, IEventsService eventsService, ITranslationService translationService)
    {
        InitializeComponent();
        _serviceProvider = serviceProvider;
        _controller = controller;
        _appInfo = appInfo;
        _translationService = translationService;
        _notificationClickHandler = null;
        // Config
        AppWindow.TitleBar.PreferredTheme = _controller.Theme switch
        {
            Theme.Light => TitleBarTheme.Light,
            Theme.Dark => TitleBarTheme.Dark,
            _ => TitleBarTheme.UseDefaultAppMode
        };
        MainGrid.RequestedTheme = _controller.Theme switch
        {
            Theme.Light => ElementTheme.Light,
            Theme.Dark => ElementTheme.Dark,
            _ => ElementTheme.Default
        };
        this.Geometry = _controller.WindowGeometry;
        // TitleBar
        AppWindow.SetIcon("./Assets/org.nickvision.application.ico");
        ExtendsContentIntoTitleBar = true;
        SetTitleBar(TitleBar);
        AppWindow.TitleBar.PreferredHeightOption = TitleBarHeightOption.Tall;
        BtnPreview.Visibility = _appInfo.Version!.IsPreview ? Visibility.Visible : Visibility.Collapsed;
        // Events
        AppWindow.Closing += Window_Closing;
        eventsService.AppNotificationSent += (sender, args) => DispatcherQueue.TryEnqueue(() => App_AppNotificationSent(sender, args));
        eventsService.ConfigurationSaved += App_ConfigurationSaved;
        eventsService.FolderChanged += App_FolderChanged;
        // Translations
        AppWindow.Title = _appInfo.ShortName;
        LblTitle.Text = _appInfo.ShortName;
        MenuFile.Title = _translationService._("File");
        MenuOpenFolder.Text = _translationService._("Open Folder");
        MenuCloseFolder.Text = _translationService._("Close Folder");
        MenuExit.Text = _translationService._("Exit");
        MenuEdit.Title = _translationService._("Edit");
        MenuSettings.Text = _translationService._("Settings");
        MenuHelp.Title = _translationService._("Help");
        MenuCheckForUpdates.Text = _translationService._("Check for Updates");
        MenuGitHubRepo.Text = _translationService._("GitHub Repo");
        MenuReportABug.Text = _translationService._("Report a Bug");
        MenuDiscussions.Text = _translationService._("Discussions");
        MenuAbout.Text = _translationService._("About {0}", _appInfo.ShortName!);
        ToolTipService.SetToolTip(BtnPreview, _translationService._("You are running a preview version of {0}", _appInfo.ShortName!));
        LblPreview.Text = _translationService._("Thank you for testing the upcoming features and changes! ❤️");
        LblHomeTitle.Text = _appInfo.ShortName;
        LblHomeDescription.Text = _appInfo.Description;
        LblOpenFolder.Text = _translationService._("Open Folder");
        LblSettings.Text = _translationService._("Settings");
        BtnCloseFolder.Label = _translationService._("Close");
    }

    private async void Window_Loaded(object sender, RoutedEventArgs args)
    {
        ViewStack.SelectedIndex = (int)Pages.Home;
        MenuCheckForUpdates.IsEnabled = false;
        await _controller.CheckForUpdatesAsync(false);
        MenuCheckForUpdates.IsEnabled = true;
    }

    private void Window_Closing(AppWindow sender, AppWindowClosingEventArgs args)
    {
        if (!_controller.CanShutdown)
        {
            args?.Cancel = true;
            return;
        }
        _controller.WindowGeometry = this.Geometry;
        _serviceProvider.GetRequiredService<IHostApplicationLifetime>().StopApplication();
    }

    private void App_AppNotificationSent(object? sender, AppNotificationSentEventArgs args)
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
        if (args.Notification.Action == "update")
        {
            BtnInfoBar.Content = _translationService._("Update");
            _notificationClickHandler = WindowsUpdate;
            BtnInfoBar.Click += _notificationClickHandler;
        }
        else if (args.Notification.Action == "close")
        {
            BtnInfoBar.Content = _translationService._("Close");
            _notificationClickHandler = CloseFolder;
            BtnInfoBar.Click += _notificationClickHandler;
        }
        BtnInfoBar.Visibility = _notificationClickHandler is not null ? Visibility.Visible : Visibility.Collapsed;
        InfoBar.IsOpen = true;
    }

    private void App_ConfigurationSaved(object? sender, ConfigurationSavedEventArgs args)
    {
        if (args.ChangedPropertyName == "Theme")
        {
            AppWindow.TitleBar.PreferredTheme = _controller.Theme switch
            {
                Theme.Light => TitleBarTheme.Light,
                Theme.Dark => TitleBarTheme.Dark,
                _ => TitleBarTheme.UseDefaultAppMode
            };
            MainGrid.RequestedTheme = _controller.Theme switch
            {
                Theme.Light => ElementTheme.Light,
                Theme.Dark => ElementTheme.Dark,
                _ => ElementTheme.Default
            };
        }
    }

    private void App_FolderChanged(object? sender, FolderChangedEventArgs args)
    {
        TitleBar.IsBackButtonVisible = false;
        ListFiles.Items.Clear();
        if (args.IsOpen)
        {
            ViewStack.SelectedIndex = (int)Pages.Folder;
            foreach (var file in args.Files)
            {
                ListFiles.Items.Add(Path.GetFileName(file));
            }
        }
        else
        {
            ViewStack.SelectedIndex = (int)Pages.Home;
        }
    }

    private void TitleBar_BackRequested(TitleBar sender, object args)
    {
        TitleBar.IsBackButtonVisible = false;
        ViewStack.SelectedIndex = ViewStack.PreviousSelectedIndex;
    }

    private async void OpenFolder(object sender, RoutedEventArgs args)
    {
        var picker = new FolderPicker(AppWindow.Id);
        var result = await picker.PickSingleFolderAsync();
        if (result is null)
        {
            return;
        }
        _controller.OpenFolder(result.Path);
    }

    private void CloseFolder(object sender, RoutedEventArgs args) => _controller.CloseFolder();

    private void Exit(object sender, RoutedEventArgs args) => Window_Closing(AppWindow, null);

    private void Settings(object sender, RoutedEventArgs args)
    {
        TitleBar.IsBackButtonVisible = true;
        ViewStack.SelectedIndex = (int)Pages.Custom;
        FrameCustom.Content = _serviceProvider.GetRequiredService<SettingsPage>();
    }

    private async void CheckForUpdates(object sender, RoutedEventArgs args)
    {
        MenuCheckForUpdates.IsEnabled = false;
        await _controller.CheckForUpdatesAsync(true);
        MenuCheckForUpdates.IsEnabled = true;
    }

    private async void GitHubRepo(object sender, RoutedEventArgs args) => await Launcher.LaunchUriAsync(_appInfo.SourceRepository!);

    private async void ReportABug(object sender, RoutedEventArgs args) => await Launcher.LaunchUriAsync(_appInfo.IssueTracker!);

    private async void Discussions(object sender, RoutedEventArgs args) => await Launcher.LaunchUriAsync(_appInfo.DiscussionsForum!);

    private async void About(object sender, RoutedEventArgs args)
    {
        var aboutDialog = _serviceProvider.GetRequiredService<AboutDialog>();
        aboutDialog.DebugInformation = _controller.GetDebugInformation();
        aboutDialog.RequestedTheme = MainGrid.ActualTheme;
        aboutDialog.XamlRoot = MainGrid.XamlRoot;
        await aboutDialog.ShowAsync();
    }

    private async void WindowsUpdate(object sender, RoutedEventArgs args)
    {
        var progress = new Progress<DownloadProgress>();
        progress.ProgressChanged += (s, p) =>
        {
            DispatcherQueue.TryEnqueue(() =>
            {
                if (p.Completed)
                {
                    FlyoutUpdateProgress.Hide();
                    BtnUpdateProgress.Visibility = Visibility.Collapsed;
                    return;
                }
                var message = _translationService._("Downloading update: {0}%", Math.Round(p.Percentage * 100));
                BtnUpdateProgress.Visibility = Visibility.Visible;
                ToolTipService.SetToolTip(BtnUpdateProgress, message);
                RingUpdateProcess.Value = p.Percentage * 100;
                LblUpdateProgress.Text = message;
            });
        };
        InfoBar.IsOpen = false;
        await _controller.WindowsUpdateAsync(progress);
    }
}