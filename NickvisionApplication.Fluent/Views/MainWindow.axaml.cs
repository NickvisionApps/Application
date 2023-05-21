using Avalonia.Controls;
using Avalonia.Interactivity;
using FluentAvalonia.UI.Controls;
using NickvisionApplication.Fluent.Controls;
using NickvisionApplication.Shared.Controllers;
using NickvisionApplication.Shared.Events;
using Symbol = FluentIcons.Common.Symbol;
using System;
using System.IO;

namespace NickvisionApplication.Fluent.Views;

/// <summary>
/// The MainWindow for the application
/// </summary>
public partial class MainWindow : Window
{
    private readonly MainWindowController _controller;
    private EventHandler<RoutedEventArgs>? _notificationButtonClickEvent;

    /// <summary>
    /// Constructs a MainWindow
    /// </summary>
    public MainWindow() 
    {
        InitializeComponent();
        _controller = new MainWindowController();
        Title = "Application";
    }

    /// <summary>
    /// Constructs a MainWindow
    /// </summary>
    /// <param name="controller">MainWindowController</param>
    public MainWindow(MainWindowController controller)
    {
        InitializeComponent();
        _controller = controller;
        Title = _controller.AppInfo.ShortName;
        //Register Events
        _controller.NotificationSent += NotificationSent;
        _controller.ShellNotificationSent += ShellNotificationSent;
        _controller.FolderChanged += FolderChanged;
        //Localize Strings
        MenuFile.Header = _controller.Localizer["File"];
        MenuOpenFolder.Header = _controller.Localizer["OpenFolder"];
        MenuCloseFolder.Header = _controller.Localizer["CloseFolder"];
        MenuExit.Header = _controller.Localizer["Exit"];
        MenuEdit.Header = _controller.Localizer["Edit"];
        MenuSettings.Header = _controller.Localizer["Settings"];
        MenuHelp.Header = _controller.Localizer["Help"];
        MenuAbout.Header = string.Format(_controller.Localizer["About"], _controller.AppInfo.ShortName);
        LblStatus.Text = _controller.Localizer["StatusReady", "WinUI"];
        StatusPageHome.Symbol = _controller.ShowSun ? Symbol.WeatherSunny : Symbol.WeatherMoon;
        StatusPageHome.Title = _controller.Greeting;
        StatusPageHome.Description = _controller.Localizer["NoFolderDescription"];
        ToolTip.SetTip(BtnHomeOpenFolder, _controller.Localizer["OpenFolder", "Tooltip"]);
        LblBtnHomeOpenFolder.Text = _controller.Localizer["Open"];
        BtnCloseFolder.Label = _controller.Localizer["CloseFolder"];
        ToolTip.SetTip(BtnCloseFolder, _controller.Localizer["CloseFolder", "Tooltip"]);
    }

    /// <summary>
    /// Occurs when a notification is sent from the controller
    /// </summary>
    /// <param name="sender">object?</param>
    /// <param name="e">NotificationSentEventArgs</param>
    private void NotificationSent(object? sender, NotificationSentEventArgs e)
    {
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
        BtnInfoBar.IsVisible = !string.IsNullOrEmpty(e.Action);
        if (e.Action == "close")
        {
            BtnInfoBar.Content = _controller.Localizer["Close"];
            _notificationButtonClickEvent = (_, _) => _controller.CloseFolder();
            BtnInfoBar.Click += _notificationButtonClickEvent;
        }
        InfoBar.IsOpen = true;
    }

    /// <summary>
    /// Occurs when a shell notification is sent from the controller
    /// </summary>
    /// <param name="sender">object?</param>
    /// <param name="e">ShellNotificationSentEventArgs</param>
    private void ShellNotificationSent(object? sender, ShellNotificationSentEventArgs e) => NotificationSent(sender, e);

    /// <summary>
    /// Occurs when the folder in the application changes
    /// </summary>
    /// <param name="sender"></param>
    /// <param name="e"></param>
    private void FolderChanged(object? sender, EventArgs e)
    {
        if(Directory.Exists(_controller.FolderPath))
        {
            Carousel.Next();
            MenuCloseFolder.IsEnabled = true;
            IconStatus.Symbol = Symbol.Folder;
        }
        else
        {
            Carousel.Previous();
            MenuCloseFolder.IsEnabled = false;
            IconStatus.Symbol = Symbol.Checkmark;
        }
        LblStatus.Text = _controller.FolderPath;
    }

    /// <summary>
    /// Occurs when the open folder menu item is clicked
    /// </summary>
    /// <param name="sender">object?</param>
    /// <param name="e">RoutedEventArgs</param>
    private async void OpenFolder(object? sender, RoutedEventArgs e)
    {
        var openFolderDialog = new OpenFolderDialog()
        {
            Title = _controller.Localizer["OpenFolder"]
        };
        var result = await openFolderDialog.ShowAsync(this);
        if (result != null)
        {
            _controller.OpenFolder(result);
        }
    }

    /// <summary>
    /// Occurs when the close folder menu item is clicked
    /// </summary>
    /// <param name="sender"></param>
    /// <param name="e"></param>
    private void CloseFolder(object? sender, RoutedEventArgs e) => _controller.CloseFolder();

    /// <summary>
    /// Occurs when the exit menu item is clicked
    /// </summary>
    /// <param name="sender">object?</param>
    /// <param name="e">RoutedEventArgs</param>
    private void Exit(object? sender, RoutedEventArgs e) => Close();

    /// <summary>
    /// Occurs when the settings menu item is clicked
    /// </summary>
    /// <param name="sender">object?</param>
    /// <param name="e">RoutedEventArgs</param>
    private void Settings(object? sender, RoutedEventArgs e)
    {
        
    }

    /// <summary>
    /// Occurs when the about menu item is clicked
    /// </summary>
    /// <param name="sender">object?</param>
    /// <param name="e">RoutedEventArgs</param>
    private async void About(object? sender, RoutedEventArgs e)
    {
        var aboutDialog = new AboutDialog(_controller.AppInfo, _controller.Localizer);
        await aboutDialog.ShowAsync(this);
    }
}