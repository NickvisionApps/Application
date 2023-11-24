using CommunityToolkit.WinUI.Controls;
using CommunityToolkit.WinUI.Notifications;
using Microsoft.UI;
using Microsoft.UI.Input;
using Microsoft.UI.Windowing;
using Microsoft.UI.Xaml;
using Microsoft.UI.Xaml.Controls;
using Microsoft.UI.Xaml.Controls.Primitives;
using Microsoft.UI.Xaml.Input;
using Microsoft.UI.Xaml.Media;
using Nickvision.Aura.Events;
using Nickvision.Aura.Taskbar;
using NickvisionApplication.Shared.Controllers;
using NickvisionApplication.WinUI.Controls;
using System;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using Windows.ApplicationModel.DataTransfer;
using Windows.Foundation;
using Windows.Graphics;
using Windows.Storage.Pickers;
using Windows.System;
using WinRT.Interop;
using static Nickvision.Aura.Localization.Gettext;

namespace NickvisionApplication.WinUI.Views;

/// <summary>
/// The MainWindow
/// </summary>
public sealed partial class MainWindow : Window
{
    private readonly MainWindowController _controller;
    private readonly IntPtr _hwnd;
    private bool _isOpened;
    private bool _isActived;
    private RoutedEventHandler? _notificationButtonClickEvent;

    private enum Monitor_DPI_Type : int
    {
        MDT_Effective_DPI = 0,
        MDT_Angular_DPI = 1,
        MDT_Raw_DPI = 2,
        MDT_Default = MDT_Effective_DPI
    }

    [DllImport("Shcore.dll", SetLastError = true)]
    private static extern int GetDpiForMonitor(IntPtr hmonitor, Monitor_DPI_Type dpiType, out uint dpiX, out uint dpiY);

    public MainWindow(MainWindowController controller)
    {
        InitializeComponent();
        _controller = controller;
        _hwnd = WindowNative.GetWindowHandle(this);
        _isOpened = false;
        _isActived = true;
        //Register Events
        AppWindow.Closing += Window_Closing;
        _controller.NotificationSent += (sender, e) => DispatcherQueue.TryEnqueue(() => NotificationSent(sender, e));
        _controller.ShellNotificationSent += ShellNotificationSent;
        _controller.FolderChanged += FolderChanged;
        //Set TitleBar
        TitleBarTitle.Text = _controller.AppInfo.ShortName;
        AppWindow.TitleBar.ExtendsContentIntoTitleBar = true;
        AppWindow.TitleBar.PreferredHeightOption = TitleBarHeightOption.Tall;
        AppWindow.TitleBar.ButtonBackgroundColor = Colors.Transparent;
        AppWindow.TitleBar.ButtonInactiveBackgroundColor = Colors.Transparent;
        AppWindow.Title = TitleBarTitle.Text;
        AppWindow.SetIcon(@"Resources\org.nickvision.application.ico");
        TitleBar.Loaded += (sender, e) => SetDragRegionForCustomTitleBar();
        TitleBar.SizeChanged += (sender, e) => SetDragRegionForCustomTitleBar();
        //Localize Strings
        TitleBarSearchBox.PlaceholderText = _("Search for files");
        TitleBarPreview.Text = _controller.AppInfo.IsDevVersion ? _("PREVIEW") : "";
        NavViewHome.PageName = _("Home");
        NavViewFolder.PageName = _("Folder");
        NavViewHelp.PageName = _("Help");
        MenuCheckForUpdates.Text = _("Check for Updates");
        MenuGitHubRepo.Text = _("GitHub Repo");
        MenuReportABug.Text = _("Report a Bug");
        MenuDiscussions.Text = _("Discussions");
        NavViewSettings.PageName = _("Settings");
        StatusPageHome.Title = _controller.Greeting;
        StatusPageHome.Description = _("Open a folder (or drag one into the app) to get started");
        HomeOpenFolderButtonLabel.Text = _("Open Folder");
        FolderOpenFolderButton.Label = _("Open");
        ToolTipService.SetToolTip(FolderCloseFolderButton, _("Close (Ctrl+W)"));
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
    /// Occurs when the window is loaded
    /// </summary>
    /// <param name="sender">object</param>
    /// <param name="e">RoutedEventArgs</param>
    private async void Window_Loaded(object sender, RoutedEventArgs e)
    {
        if (!_isOpened)
        {
            NavView.IsEnabled = false;
            ViewStack.CurrentPageName = "Spinner";
            var accent = (SolidColorBrush)Application.Current.Resources["AccentFillColorDefaultBrush"];
            _controller.TaskbarItem = TaskbarItem.ConnectWindows(_hwnd, new System.Drawing.SolidBrush(System.Drawing.Color.FromArgb(accent.Color.A, accent.Color.R, accent.Color.G, accent.Color.B)), MainGrid.ActualTheme == ElementTheme.Dark ? System.Drawing.Brushes.Black : System.Drawing.Brushes.White);
            await _controller.StartupAsync();
            NavView.IsEnabled = true;
            NavViewHome.IsSelected = true;
            _isOpened = true;
        }
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
    /// Occurs when something is dropped into the window
    /// </summary>
    /// <param name="sender">object</param>
    /// <param name="e">DragEventArgs</param>
    private async void OnDrop(object sender, DragEventArgs e)
    {
        if (e.DataView.Contains(StandardDataFormats.StorageItems))
        {
            var first = (await e.DataView.GetStorageItemsAsync()).FirstOrDefault();
            if (first != null)
            {
                _controller.OpenFolder(first.Path);
            }
        }
    }

    /// <summary>
    /// Occurs when something is dragged over into the window
    /// </summary>
    /// <param name="sender">object</param>
    /// <param name="e">DragEventArgs</param>
    private void OnDragOver(object sender, DragEventArgs e)
    {
        e.AcceptedOperation = DataPackageOperation.Copy | DataPackageOperation.Link;
        e.DragUIOverride.Caption = _("Drop here to open folder");
        e.DragUIOverride.IsGlyphVisible = true;
        e.DragUIOverride.IsContentVisible = true;
        e.DragUIOverride.IsCaptionVisible = true;
    }

    /// <summary>
    /// Occurs when the NavView's selection has changed
    /// </summary>
    /// <param name="sender">NavigationView</param>
    /// <param name="args">NavigationViewSelectionChangedEventArgs</param>
    private void NavView_SelectionChanged(NavigationView sender, NavigationViewSelectionChangedEventArgs args)
    {
        var tag = (NavView.SelectedItem as NavigationViewItem)!.Tag as string;
        if(tag == "Home")
        {
            ViewStack.CurrentPageName = "Home";
        }
        else if (tag == "Folder")
        {
            ViewStack.CurrentPageName = "Folder";
        }
        else if(tag == "Settings")
        {
            ViewStack.CurrentPageName = "Custom";
            FrameCustom.Content = new SettingsPage(_controller.CreatePreferencesViewController());
        }
        TitleBarSearchBox.Visibility = tag == "Folder" ? Visibility.Visible : Visibility.Collapsed;
        SetDragRegionForCustomTitleBar();
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
        if (_notificationButtonClickEvent != null)
        {
            BtnInfoBar.Click -= _notificationButtonClickEvent;
        }
        if (e.Action == "update")
        {
            _notificationButtonClickEvent = WindowsUpdate;
            BtnInfoBar.Content = _("Update");
            BtnInfoBar.Click += _notificationButtonClickEvent;
        }
        else if (e.Action == "close")
        {
            _notificationButtonClickEvent = CloseFolder;
            BtnInfoBar.Content = _("Close");
            BtnInfoBar.Click += _notificationButtonClickEvent;
        }
        BtnInfoBar.Visibility = !string.IsNullOrEmpty(e.Action) ? Visibility.Visible : Visibility.Collapsed;
        InfoBar.IsOpen = true;
    }

    /// <summary>
    /// Occurs when a shell notification is sent from the controller
    /// </summary>
    /// <param name="sender">object?</param>
    /// <param name="e">ShellNotificationSentEventArgs</param>
    private void ShellNotificationSent(object? sender, ShellNotificationSentEventArgs e) => new ToastContentBuilder().AddText(e.Title).AddText(e.Message).Show();

    /// <summary>
    /// Occurs when the folder is changed
    /// </summary>
    /// <param name="sender">object?</param>
    /// <param name="e">EventArgs</param>
    private void FolderChanged(object? sender, EventArgs e)
    {
        NavViewHome.IsSelected = !_controller.IsFolderOpened;
        NavViewFolder.IsEnabled = _controller.IsFolderOpened;
        NavViewFolder.IsSelected = _controller.IsFolderOpened;
        ListFiles.Items.Clear();
        if (_controller.IsFolderOpened)
        {
            StatusPageFiles.Description = _n("There is {0} file in the folder.", "There are {0} files in the folder.", _controller.FilesCount);
            foreach(var file in _controller.Files)
            {
                ListFiles.Items.Add(new StackPanel()
                {
                    Margin = new Thickness(6, 6, 6, 6),
                    Orientation = Orientation.Vertical,
                    Spacing = 6,
                    Children =
                    {
                        new TextBlock() { Text = Path.GetFileName(file) },
                        new TextBlock() { Text = file, Foreground = new SolidColorBrush(Colors.Gray) }
                    }
                });
            }
        }
    }

    /// <summary>
    /// Occurs when the open folder menu item is clicked
    /// </summary>
    /// <param name="sender">object</param>
    /// <param name="e">RoutedEventArgs</param>
    private async void OpenFolder(object sender, RoutedEventArgs e)
    {
        var folderPicker = new FolderPicker();
        InitializeWithWindow(folderPicker);
        folderPicker.FileTypeFilter.Add("*");
        var folder = await folderPicker.PickSingleFolderAsync();
        if (folder != null)
        {
            _controller.OpenFolder(folder.Path);
        }
    }

    /// <summary>
    /// Occurs when the close folder menu item is clicked
    /// </summary>
    /// <param name="sender">object</param>
    /// <param name="e">RoutedEventArgs</param>
    private void CloseFolder(object sender, RoutedEventArgs e)
    {
        InfoBar.IsOpen = false;
        _controller.CloseFolder();
    }

    /// <summary>
    /// Occurs when the NavViewHelp item is tapped
    /// </summary>
    /// <param name="sender">object</param>
    /// <param name="e">TappedRoutedEventArgs</param>
    private void NavViewHelp_Tapped(object sender, TappedRoutedEventArgs e) => FlyoutBase.ShowAttachedFlyout((FrameworkElement)sender);

    /// <summary>
    /// Occurs when the check for updates menu item is clicked
    /// </summary>
    /// <param name="sender">object</param>
    /// <param name="e">RoutedEventArgs</param>
    private async void CheckForUpdates(object sender, RoutedEventArgs e) => await _controller.CheckForUpdatesAsync();

    /// <summary>
    /// Occurs when the windows update button is clicked
    /// </summary>
    /// <param name="sender">object</param>
    /// <param name="e">RoutedEventArgs</param>
    private async void WindowsUpdate(object sender, RoutedEventArgs e)
    {
        NavView.IsEnabled = false;
        var searchVisibility = TitleBarSearchBox.Visibility;
        var page = ViewStack.CurrentPageName;
        TitleBarSearchBox.Visibility = Visibility.Collapsed;
        ViewStack.CurrentPageName = "Spinner";
        SetDragRegionForCustomTitleBar();
        if (!(await _controller.WindowsUpdateAsync()))
        {
            NavView.IsEnabled = true;
            TitleBarSearchBox.Visibility = searchVisibility;
            ViewStack.CurrentPageName = page;
            SetDragRegionForCustomTitleBar();
        }
    }

    /// <summary>
    /// Occurs when the github repo menu item is clicked
    /// </summary>
    /// <param name="sender">object</param>
    /// <param name="e">RoutedEventArgs</param>
    private async void GitHubRepo(object sender, RoutedEventArgs e) => await Launcher.LaunchUriAsync(_controller.AppInfo.SourceRepo);

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
    /// Sets the drag region for the TitleBar
    /// </summary>
    private void SetDragRegionForCustomTitleBar()
    {
        double scaleAdjustment = TitleBar.XamlRoot.RasterizationScale;
        RightPaddingColumn.Width = new GridLength(AppWindow.TitleBar.RightInset / scaleAdjustment);
        LeftPaddingColumn.Width = new GridLength(AppWindow.TitleBar.LeftInset / scaleAdjustment);
        var transform = TitleBarSearchBox.TransformToVisual(null);
        var bounds = transform.TransformBounds(new Rect(0, 0, TitleBarSearchBox.ActualWidth, TitleBarSearchBox.ActualHeight));
        var searchBoxRect = new RectInt32((int)Math.Round(bounds.X * scaleAdjustment), (int)Math.Round(bounds.Y * scaleAdjustment), (int)Math.Round(bounds.Width * scaleAdjustment), (int)Math.Round(bounds.Height * scaleAdjustment));
        transform = TitleBarPreview.TransformToVisual(null);
        bounds = transform.TransformBounds(new Rect(0, 0, TitleBarPreview.ActualWidth, TitleBarPreview.ActualHeight));
        var previewRect = new RectInt32((int)Math.Round(bounds.X * scaleAdjustment), (int)Math.Round(bounds.Y * scaleAdjustment), (int)Math.Round(bounds.Width * scaleAdjustment), (int)Math.Round(bounds.Height * scaleAdjustment));
        var rectArray = new RectInt32[] { searchBoxRect, previewRect };
        var nonClientInputSrc = InputNonClientPointerSource.GetForWindowId(AppWindow.Id);
        nonClientInputSrc.SetRegionRects(NonClientRegionKind.Passthrough, rectArray);
    }
}
