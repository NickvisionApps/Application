using Microsoft.Extensions.DependencyInjection;
using Microsoft.Extensions.Hosting;
using Microsoft.UI.Windowing;
using Microsoft.UI.Xaml;
using Microsoft.UI.Xaml.Controls;
using Microsoft.UI.Xaml.Controls.Primitives;
using Microsoft.UI.Xaml.Input;
using Microsoft.Windows.AppNotifications;
using Microsoft.Windows.AppNotifications.Builder;
using Microsoft.Windows.Storage.Pickers;
using Nickvision.Application.Shared.Controllers;
using Nickvision.Application.Shared.Events;
using Nickvision.Application.Shared.Models;
using Nickvision.Application.Shared.Services;
using Nickvision.Application.WinUI.Controls;
using Nickvision.Desktop.Application;
using Nickvision.Desktop.Filesystem;
using Nickvision.Desktop.Globalization;
using Nickvision.Desktop.Network;
using Nickvision.Desktop.Notifications;
using Nickvision.Desktop.WinUI.Helpers;
using System;
using System.IO;
using System.Threading.Tasks;
using Windows.Storage;
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
        // Events
        AppWindow.Closing += Window_Closing;
        eventsService.AppNotificationSent += (sender, args) => DispatcherQueue.TryEnqueue(() => Controller_AppNotificationSent(sender, args));
        eventsService.FolderChanged += Controller_FolderChanged;
        eventsService.JsonFileSaved += Controller_JsonFileSaved;
        // Translations
        AppWindow.Title = _appInfo.ShortName;
        TitleBar.Title = _appInfo.ShortName;
        TitleBar.Subtitle = _appInfo.Version!.IsPreview ? _translationService._("Preview") : string.Empty;
        NavItemHome.Content = _translationService._("Home");
        NavItemFolder.Content = _translationService._("Folder");
        NavItemUpdates.Content = _translationService._("Updating");
        NavItemHelp.Content = _translationService._("Help");
        MenuCheckForUpdates.Text = _translationService._("Check for Updates");
        MenuGitHubRepo.Text = _translationService._("GitHub Repo");
        MenuReportABug.Text = _translationService._("Report a Bug");
        MenuDiscussions.Text = _translationService._("Discussions");
        MenuAbout.Text = _translationService._("About {0}", _appInfo.ShortName!);
        NavItemSettings.Content = _translationService._("Settings");
        StatusHome.Title = _controller.Greeting;
        StatusHome.Description = _translationService._("Open a folder to get started");
        LblHomeOpenFolder.Text = _translationService._("Open Folder");
        BtnFolderOpenFolder.Label = _translationService._("Open");
        BtnFolderCloseFolder.Label = _translationService._("Close");
    }

    private async void Window_Loaded(object sender, RoutedEventArgs e)
    {
        MenuCheckForUpdates.IsEnabled = false;
        await _controller.CheckForUpdatesAsync(false);
        MenuCheckForUpdates.IsEnabled = true;
    }

    private void Window_Closing(AppWindow sender, AppWindowClosingEventArgs args)
    {
        if (!_controller.CanShutdown)
        {
            args.Cancel = true;
            return;
        }
        _controller.WindowGeometry = this.Geometry;
        _serviceProvider.GetRequiredService<IHostApplicationLifetime>().StopApplication();
    }

    private void TitleBar_PaneToggleRequested(TitleBar sender, object args)
    {
        NavView.IsPaneOpen = !NavView.IsPaneOpen;
    }

    private void NavView_SelectionChanged(NavigationView sender, NavigationViewSelectionChangedEventArgs args)
    {
        if (args.SelectedItem is NavigationViewItem item)
        {
            var tag = item.Tag as string;
            FrameCustom.Content = tag switch
            {
                "Settings" => _serviceProvider.GetRequiredService<SettingsPage>(),
                _ => null
            };
            ViewStack.SelectedIndex = tag switch
            {
                "Folder" => (int)Pages.Folder,
                "Settings" => (int)Pages.Custom,
                _ => (int)Pages.Home
            };
        }
    }

    private void NavItem_Tapped(object sender, TappedRoutedEventArgs e) => FlyoutBase.ShowAttachedFlyout(sender as FrameworkElement);

    private void Controller_AppNotificationSent(object? sender, AppNotificationSentEventArgs args)
    {
        if (args.Notification is ShellNotification shellNotification)
        {
            var builder = new AppNotificationBuilder()
                .AddText(shellNotification.Title)
                .AddText(shellNotification.Message);
            if (shellNotification.Action == "open")
            {
                builder.AddButton(new AppNotificationButton(_translationService._("Open in Explorer"))
                    .AddArgument("action", "OpenInExplorer")
                    .AddArgument("param", shellNotification.ActionParam));
            }
            AppNotificationManager.Default.Show(builder.BuildNotification());
            return;
        }
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

    private void Controller_JsonFileSaved(object? sender, JsonFileSavedEventArgs args)
    {
        if (args.Name == Configuration.Key)
        {
            MainGrid.RequestedTheme = _controller.Theme switch
            {
                Theme.Light => ElementTheme.Light,
                Theme.Dark => ElementTheme.Dark,
                _ => ElementTheme.Default
            };
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

    private async void CheckForUpdates(object sender, RoutedEventArgs e)
    {
        MenuCheckForUpdates.IsEnabled = false;
        await _controller.CheckForUpdatesAsync(true);
        MenuCheckForUpdates.IsEnabled = true;
    }

    private async void GitHubRepo(object sender, RoutedEventArgs e) => await LaunchUriAsync(_appInfo.SourceRepository);

    private async void ReportABug(object sender, RoutedEventArgs e) => await LaunchUriAsync(_appInfo.IssueTracker);

    private async void Discussions(object sender, RoutedEventArgs e) => await LaunchUriAsync(_appInfo.DiscussionsForum);

    private async void About(object sender, RoutedEventArgs e)
    {
        var aboutDialog = _serviceProvider.GetRequiredService<AboutDialog>();
        aboutDialog.DebugInformation = _controller.GetDebugInformation();
        aboutDialog.RequestedTheme = MainGrid.ActualTheme;
        aboutDialog.XamlRoot = MainGrid.XamlRoot;
        await aboutDialog.ShowAsync();
    }

    private async void WindowsUpdate(object sender, RoutedEventArgs e)
    {
        var progress = new Progress<DownloadProgress>();
        progress.ProgressChanged += (s, p) =>
        {
            DispatcherQueue.TryEnqueue(() =>
            {
                if (p.Completed)
                {
                    FlyoutProgress.Hide();
                    NavItemUpdates.Visibility = Visibility.Collapsed;
                    return;
                }
                var message = _translationService._("Downloading update: {0}%", Math.Round(p.Percentage * 100));
                NavItemUpdates.Visibility = Visibility.Visible;
                StsProgress.Description = message;
                BarProgress.Value = p.Percentage * 100;
            });
        };
        InfoBar.IsOpen = false;
        await _controller.WindowsUpdateAsync(progress);
    }

    private async Task LaunchUriAsync(Uri? uri)
    {
        if (uri is null)
        {
            return;
        }
        await Launcher.LaunchUriAsync(uri);
    }
}