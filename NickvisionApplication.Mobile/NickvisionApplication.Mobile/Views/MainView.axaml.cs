using Avalonia.Controls;
using Avalonia.Controls.ApplicationLifetimes;
using Avalonia.Input;
using Avalonia.Interactivity;
using Avalonia.Platform.Storage;
using FluentAvalonia.UI.Controls;
using NickvisionApplication.Mobile.Controls;
using NickvisionApplication.Mobile.Helpers;
using NickvisionApplication.Shared.Controllers;
using NickvisionApplication.Shared.Events;
using System;
using System.Globalization;
using System.IO;
using static NickvisionApplication.Shared.Helpers.Gettext;

namespace NickvisionApplication.Mobile.Views;

/// <summary>
/// The MainView for the application
/// </summary>
public partial class MainView : UserControl
{
    private readonly MainWindowController _controller;
    private readonly IApplicationLifetime _lifetime;
    private bool _opened;
    
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
        //Register Events
        _controller.NotificationSent += NotificationSent;
        _controller.ShellNotificationSent += ShellNotificationSent;
        _controller.FolderChanged += FolderChanged;
        //NavView
        HomeNavItem.Content = _("Home");
        SettingsNavItem.Content = _("Settings");
        AboutNavItem.Content = _("About Application");
        //Greeting
        StatusPageHome.Symbol = Symbol.Home;
        StatusPageHome.Title = _controller.Greeting;
        StatusPageHome.Description = _("Open a folder to get started");
        BtnHomeOpenFolderLabel.Text = _("Open");
        //About Page
        LblChangelogHeader.Text = _("Changelog");
        LblGitHubHeader.Text = _("GitHub Repo");
        LblReportABug.Text = _("Report a Bug");
        LblDiscussions.Text = _("Discussions");
        LblCreditsHeader.Text = _("Credits");
        LblDescription.Text = _controller.AppInfo.Description;
        LblVersion.Text = _controller.AppInfo.Version;
        LblChangelog.Text = _controller.AppInfo.Changelog;
        var credits = string.Format(_("Developers:\n{0}\n\nDesigners:\n{1}\n\nArtists:\n{2}\n\nTranslators:\n{3}"),
            string.Join('\n', _controller.AppInfo.Developers.Keys), string.Join('\n', _controller.AppInfo.Designers.Keys),
            string.Join('\n', _controller.AppInfo.Artists.Keys), _controller.AppInfo.TranslatorCredits);
        if (string.IsNullOrEmpty(_controller.AppInfo.TranslatorCredits))
        {
            credits = credits.Remove(credits.TrimEnd().LastIndexOf('\n'));
        }
        LblCredits.Text = credits;
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
            
        }
        else
        {
            
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
            _controller.OpenFolder(result[0].TryGetLocalPath() ?? "");
        }
    }

    /// <summary>
    /// Occurs when the close folder button is clicked
    /// </summary>
    /// <param name="sender">object?</param>
    /// <param name="e">RoutedEventArgs</param>
    private void CloseFolder(object? sender, RoutedEventArgs e) => _controller.CloseFolder();

    /// <summary>
    /// Occurs when the NavView's selection changes
    /// </summary>
    /// <param name="sender">object?</param>
    /// <param name="e">NavigationViewSelectionChangedEventArgs</param>
    private void NavigationView_OnSelectionChanged(object? sender, NavigationViewSelectionChangedEventArgs e) => ViewStack.CurrentPageName = (e.SelectedItem as NavigationViewItem)!.Tag as string ?? "";
    
    /// <summary>
    /// Occurs when the version button is clicked
    /// </summary>
    /// <param name="sender">object?</param>
    /// <param name="e">RoutedEventArgs</param>
    private async void CopySystemInformation(object? sender, RoutedEventArgs e)
    {
        if (TopLevel.GetTopLevel(this) != null && TopLevel.GetTopLevel(this)!.Clipboard != null)
        {
            await TopLevel.GetTopLevel(this)!.Clipboard!.SetTextAsync($"{_controller.AppInfo.ID}\n{_controller.AppInfo.Version}\n\n{Environment.OSVersion}\n{CultureInfo.CurrentCulture.Name}");
            NotificationSent(sender, new NotificationSentEventArgs(_("Copied system information."), NotificationSeverity.Success));
        }
    }
    
    /// <summary>
    /// Occurs when the github repo button is clicked
    /// </summary>
    /// <param name="sender">object</param>
    /// <param name="e">RoutedEventArgs</param>
    private void GitHubRepo(object sender, RoutedEventArgs e) => _controller.AppInfo.SourceRepo.OpenInBrowser();
    
    /// <summary>
    /// Occurs when the report a bug button is clicked
    /// </summary>
    /// <param name="sender">object</param>
    /// <param name="e">RoutedEventArgs</param>
    private void ReportABug(object sender, RoutedEventArgs e) => _controller.AppInfo.IssueTracker.OpenInBrowser();
    
    /// <summary>
    /// Occurs when the discussions button is clicked
    /// </summary>
    /// <param name="sender">object</param>
    /// <param name="e">RoutedEventArgs</param>
    private void Discussions(object sender, RoutedEventArgs e) => _controller.AppInfo.SupportUrl.OpenInBrowser();
}