using Avalonia.Controls;
using Avalonia.Controls.ApplicationLifetimes;
using Avalonia.Interactivity;
using Avalonia.Platform.Storage;
using FluentAvalonia.UI.Controls;
using NickvisionApplication.Avalonia.Controls;
using NickvisionApplication.Shared.Controllers;
using NickvisionApplication.Shared.Events;
using System;
using System.IO;
using static NickvisionApplication.Shared.Helpers.Gettext;

namespace NickvisionApplication.Avalonia.Views;

/// <summary>
/// The MainView for the application
/// </summary>
public partial class MainView : UserControl
{
    private readonly MainWindowController _controller;
    private readonly IApplicationLifetime _lifetime;
    private bool _opened;
    private int _page;
    
    /// <summary>
    /// Constructs a MainView
    /// </summary>
    /// <param name="controller">MainWindowController</param>
    /// <param name="lifetime">IApplicationLifetime</param>
    public MainView(MainWindowController controller, IApplicationLifetime lifetime)
    {
        InitializeComponent();
        InitializeComponent();
        _controller = controller;
        _lifetime = lifetime;
        _opened = false;
        _page = 0;
        //Register Events
        _controller.NotificationSent += NotificationSent;
        _controller.ShellNotificationSent += ShellNotificationSent;
        _controller.FolderChanged += FolderChanged;
        //Header
        BtnOpenFolderLabel.Text = _("Open");
        ToolTip.SetTip(BtnOpenFolder, _("Open Folder (Ctrl+O)"));
        ToolTip.SetTip(BtnCloseFolder, _("Close Folder (Ctrl+W)"));
        ToolTip.SetTip(BtnSettings, _("Settings"));
        ToolTip.SetTip(BtnAbout, _("About Application"));
        //Greeting
        StatusPageHome.Symbol = Symbol.Home;
        StatusPageHome.Title = _controller.Greeting;
        StatusPageHome.Description = _("Open a folder to get started");
        BtnHomeOpenFolderLabel.Text = _("Open");
    }
    
    /// <summary>
    /// Occurs when the main view is loaded
    /// </summary>
    /// <param name="sender">object?</param>
    /// <param name="e">RoutedEventArgs</param>
    private void OnLoaded(object? sender, RoutedEventArgs e)
    {
        if (!_opened)
        {
            _controller.Startup(_lifetime is IClassicDesktopStyleApplicationLifetime desktop ? (desktop.Args ?? Array.Empty<string>()) : Array.Empty<string>());
            _opened = true;
        }
    }

    /// <summary>
    /// Occurs when a notification is sent from the controller
    /// </summary>
    /// <param name="sender">object?</param>
    /// <param name="e">NotificationSentEventArgs</param>
    private void NotificationSent(object? sender, NotificationSentEventArgs e)
    {
        InfoBar.Title = e.Message;
        InfoBar.Severity = e.Severity switch
        {
            NotificationSeverity.Success => InfoBarSeverity.Success,
            NotificationSeverity.Error => InfoBarSeverity.Error,
            NotificationSeverity.Warning => InfoBarSeverity.Warning,
            _ => InfoBarSeverity.Informational
        };
        if (e.Action == "close")
        {
            var button = new Button()
            {
                Content = _("Close")
            };
            button.Click += CloseFolder;
            InfoBar.ActionButton = button;
        }
        else
        {
            InfoBar.ActionButton = null;
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
    /// Occurs when the folder is changed
    /// </summary>
    /// <param name="sender">object?</param>
    /// <param name="e">EventArgs</param>
    private void FolderChanged(object? sender, EventArgs e)
    {
        if (_controller.IsFolderOpened)
        {
            if (_page != 1)
            {
                Carousel.Next();
                _page++;
            }
            BtnCloseFolder.IsVisible = true;
            FilesLabel.Text = _n("There is {0} file in the folder.", "There are {0} files in the folder.", Directory.GetFiles(_controller.FolderPath, "*", SearchOption.TopDirectoryOnly).Length);
        }
        else
        {
            if (_page != 0)
            {
                Carousel.Previous();
                _page--;
            }
            BtnCloseFolder.IsVisible = false;
        }
    }
    
    /// <summary>
    /// Occurs when the open folder button is clicked
    /// </summary>
    /// <param name="sender">object?</param>
    /// <param name="e">RoutedEventArgs</param>
    private async void OpenFolder(object? sender, RoutedEventArgs e)
    {
        var result = await TopLevel.GetTopLevel(this)!.StorageProvider.OpenFolderPickerAsync(new FolderPickerOpenOptions()
        {
            Title = _("Open Folder"),
            AllowMultiple = false
        });
        if (result.Count == 1)
        {
            var bookmark = await result[0].SaveBookmarkAsync() ?? "";
            _controller.OpenFolder((await TopLevel.GetTopLevel(this)!.StorageProvider.OpenFolderBookmarkAsync(bookmark))!.TryGetLocalPath() ?? "");
        }
    }

    /// <summary>
    /// Occurs when the close folder button is clicked
    /// </summary>
    /// <param name="sender">object?</param>
    /// <param name="e">RoutedEventArgs</param>
    private void CloseFolder(object? sender, RoutedEventArgs e) => _controller.CloseFolder();

    /// <summary>
    /// Occurs when the settings button is clicked
    /// </summary>
    /// <param name="sender">object?</param>
    /// <param name="e">RoutedEventArgs</param>
    private void Settings(object? sender, RoutedEventArgs e)
    {
        
    }
    
    /// <summary>
    /// Occurs when the about button is clicked
    /// </summary>
    /// <param name="sender">object?</param>
    /// <param name="e">RoutedEventArgs</param>
    private async void About(object? sender, RoutedEventArgs e)
    {
        var aboutDialog = new AboutDialog(_controller.AppInfo);
        await aboutDialog.ShowAsync(TopLevel.GetTopLevel(this));
    }
}