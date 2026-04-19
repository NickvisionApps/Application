using Microsoft.Extensions.DependencyInjection;
using Microsoft.Extensions.Hosting;
using Nickvision.Application.Shared.Controllers;
using Nickvision.Application.Shared.Events;
using Nickvision.Application.Shared.Models;
using Nickvision.Application.Shared.Services;
using Nickvision.Desktop.Application;
using Nickvision.Desktop.Globalization;
using Nickvision.Desktop.GNOME.Controls;
using Nickvision.Desktop.GNOME.Helpers;
using Nickvision.Desktop.Notifications;
using System;
using System.Diagnostics.CodeAnalysis;
using System.IO;
using System.Linq;
using System.Threading.Tasks;

namespace Nickvision.Application.GNOME.Views;

public class MainWindow : Adw.ApplicationWindow
{
    private readonly IServiceProvider _serviceProvider;
    private readonly Adw.Application _application;
    private readonly MainWindowController _controller;
    private readonly AppInfo _appInfo;
    private readonly ITranslationService _translationService;
    private readonly Gtk.Builder _builder;
    private bool _shown;

    [Gtk.Connect("windowTitle")]
    private Adw.WindowTitle? _windowTitle;
    [Gtk.Connect("btnOpenFolder")]
    private Gtk.Button? _btnOpenFolder;
    [Gtk.Connect("btnCloseFolder")]
    private Gtk.Button? _btnCloseFolder;
    [Gtk.Connect("toastOverlay")]
    private Adw.ToastOverlay? _toastOverlay;
    [Gtk.Connect("viewStack")]
    private Adw.ViewStack? _viewStack;
    [Gtk.Connect("pageGreeting")]
    private Adw.StatusPage? _pageGreeting;
    [Gtk.Connect("pageFiles")]
    private Adw.StatusPage? _pageFiles;

    public MainWindow(IServiceProvider serviceProvider, Adw.Application application, MainWindowController controller, AppInfo appInfo, IEventsService eventsService, ITranslationService translationService, IGtkBuilderFactory builderFactory) : this(serviceProvider, application, controller, appInfo, eventsService, translationService, builderFactory.Create("MainWindow"))
    {

    }

    [DynamicDependency(DynamicallyAccessedMemberTypes.NonPublicFields, typeof(MainWindow))]
    private MainWindow(IServiceProvider serviceProvider, Adw.Application application, MainWindowController controller, AppInfo appInfo, IEventsService eventsService, ITranslationService translationService, Gtk.Builder builder) : base(new Adw.Internal.ApplicationWindowHandle(builder.GetPointer("root"), false))
    {
        _serviceProvider = serviceProvider;
        _application = application;
        _controller = controller;
        _appInfo = appInfo;
        _translationService = translationService;
        _builder = builder;
        _shown = false;
        _builder.Connect(this);
        // Window
        Adw.StyleManager.GetDefault().ColorScheme = _controller.Theme switch
        {
            Theme.Light => Adw.ColorScheme.ForceLight,
            Theme.Dark => Adw.ColorScheme.ForceDark,
            _ => Adw.ColorScheme.Default
        };
        Title = _appInfo.ShortName;
        IconName = _appInfo.Id;
        if (_appInfo.Version!.IsPreview)
        {
            AddCssClass("devel");
        }
        _windowTitle!.Title = _appInfo.ShortName;
        _pageGreeting!.Title = _controller.Greeting;
        // Events
        OnShow += Window_OnShow;
        OnCloseRequest += Window_OnCloseRequest;
        eventsService.AppNotificationSent += (sender, e) => GLib.Functions.IdleAdd(0, () =>
        {
            App_AppNotificationSent(sender, e);
            return false;
        });
        eventsService.FolderChanged += App_FolderChanged;
        // Drop target
        var dropTarget = Gtk.DropTarget.New(Gio.FileHelper.GetGType(), Gdk.DragAction.Copy);
        dropTarget.OnDrop += Window_OnDrop;
        AddController(dropTarget);
        // Quit action
        var actQuit = Gio.SimpleAction.New("quit", null);
        actQuit.OnActivate += Quit;
        AddAction(actQuit);
        _application.SetAccelsForAction("win.quit", ["<Ctrl>q"]);
        // Open in files action
        var actOpenInFiles = Gio.SimpleAction.New("openInFiles", null);
        actOpenInFiles.OnActivate += OpenInFiles;
        AddAction(actOpenInFiles);
        // Open folder action
        var actOpenFolder = Gio.SimpleAction.New("openFolder", null);
        actOpenFolder.OnActivate += OpenFolder;
        AddAction(actOpenFolder);
        _application.SetAccelsForAction("win.openFolder", ["<Ctrl>o"]);
        // Close folder action
        var actCloseFolder = Gio.SimpleAction.New("closeFolder", null);
        actCloseFolder.OnActivate += CloseFolder;
        AddAction(actCloseFolder);
        _application.SetAccelsForAction("win.closeFolder", ["<Ctrl>w"]);
        // Preferences action
        var actPreferences = Gio.SimpleAction.New("preferences", null);
        actPreferences.OnActivate += Preferences;
        AddAction(actPreferences);
        _application.SetAccelsForAction("win.preferences", ["<Ctrl>period"]);
        // Keyboard shortcuts action
        var actKeyboardShortcuts = Gio.SimpleAction.New("keyboardShortcuts", null);
        actKeyboardShortcuts.OnActivate += KeyboardShortcuts;
        AddAction(actKeyboardShortcuts);
        _application.SetAccelsForAction("win.keyboardShortcuts", ["<Ctrl>question"]);
        // About action
        var actAbout = Gio.SimpleAction.New("about", null);
        actAbout.OnActivate += About;
        AddAction(actAbout);
        _application.SetAccelsForAction("win.about", ["F1"]);
    }

    public new void Present()
    {
        base.Present();
        this.WindowGeometry = _controller.WindowGeometry;
    }

    public async void Window_OnShow(Gtk.Widget sender, EventArgs e)
    {
        if (_shown)
        {
            return;
        }
        await _controller.CheckForUpdatesAsync(false);
        _shown = true;
    }

    private bool Window_OnCloseRequest(Gtk.Window sender, EventArgs e)
    {
        if (!_controller.CanShutdown)
        {
            return true;
        }
        GetDefaultSize(out int width, out int height);
        _controller.WindowGeometry = this.WindowGeometry;
        Destroy();
        _serviceProvider.GetRequiredService<IHostApplicationLifetime>().StopApplication();
        return false;
    }

    private bool Window_OnDrop(Gtk.DropTarget sender, Gtk.DropTarget.DropSignalArgs e)
    {
        var file = new Gio.FileHelper(e.Value.GetObject()!.Handle);
        var path = file.GetPath();
        if (string.IsNullOrEmpty(path))
        {
            return false;
        }
        _controller.OpenFolder(path);
        return true;
    }

    private void App_AppNotificationSent(object? sender, AppNotificationSentEventArgs e)
    {
        if (e.Notification is ShellNotification shellNotification)
        {
            var notification = Gio.Notification.New(shellNotification.Title);
            notification.SetBody(shellNotification.Message);
            if (shellNotification.Action == "open" && !Directory.Exists(shellNotification.ActionParam))
            {
                notification.AddButton(_translationService._("Open in Files"), "win.openInFiles");
            }
            _application.SendNotification(null, notification);
        }
        var toast = Adw.Toast.New(e.Notification.Message);
        if (e.Notification.Action == "close")
        {
            toast.ButtonLabel = _translationService._("Close");
            toast.OnButtonClicked += (_, _) => _controller.CloseFolder();
        }
        else if (e.Notification.Action == "update")
        {
            toast.ButtonLabel = _translationService._("View");
            toast.OnButtonClicked += async (_, _) =>
            {
                var launcher = Gtk.UriLauncher.New($"{_appInfo.SourceRepository}/releases/latest");
                await launcher.LaunchAsync(this);
            };
        }
        _toastOverlay!.AddToast(toast);
    }

    private void App_FolderChanged(object? sender, FolderChangedEventArgs e)
    {
        _windowTitle!.Subtitle = e.IsOpen ? e.Path : string.Empty;
        _btnOpenFolder!.Visible = e.IsOpen;
        _btnCloseFolder!.Visible = e.IsOpen;
        _viewStack!.VisibleChildName = e.IsOpen ? "Folder" : "NoFolder";
        _pageFiles!.Description = _translationService._n("There is {0} file in the folder.", "There are {0} files in the folder", e.Files.Count, e.Files.Count);
    }

    private void Quit(Gio.SimpleAction sender, Gio.SimpleAction.ActivateSignalArgs e) => Window_OnCloseRequest(this, new EventArgs());

    private async void OpenInFiles(Gio.SimpleAction sender, Gio.SimpleAction.ActivateSignalArgs e)
    {
        if (!Directory.Exists(_controller.CurrentFolder))
        {
            return;
        }
        var launcher = Gtk.FileLauncher.New(Gio.FileHelper.NewForPath(_controller.CurrentFolder));
        await launcher.LaunchAsync(this);
    }

    private async void OpenFolder(Gio.SimpleAction sender, Gio.SimpleAction.ActivateSignalArgs e)
    {
        var folderDialog = Gtk.FileDialog.New();
        folderDialog.Title = _translationService._("Open Folder");
        try
        {
            var file = await folderDialog.SelectFolderAsync(this);
            if (file is not null)
            {
                _controller.OpenFolder(file.GetPath()!);
            }
        }
        catch { }
    }

    private void CloseFolder(Gio.SimpleAction sender, Gio.SimpleAction.ActivateSignalArgs e) => _controller.CloseFolder();

    private void Preferences(Gio.SimpleAction sender, Gio.SimpleAction.ActivateSignalArgs e) => _serviceProvider.GetRequiredService<PreferencesDialog>().Present(this);

    private void KeyboardShortcuts(Gio.SimpleAction sender, Gio.SimpleAction.ActivateSignalArgs e) => _serviceProvider.GetRequiredService<ShortcutsDialog>().Present(this);

    private void About(Gio.SimpleAction sender, Gio.SimpleAction.ActivateSignalArgs e)
    {
        var extraInfo = string.Empty;
        extraInfo += $"GTK {Gtk.Functions.GetMajorVersion()}.{Gtk.Functions.GetMinorVersion()}.{Gtk.Functions.GetMicroVersion()}\n";
        extraInfo += $"libadwaita {Adw.Functions.GetMajorVersion()}.{Adw.Functions.GetMinorVersion()}.{Adw.Functions.GetMicroVersion()}";
        var dialog = Adw.AboutDialog.New();
        dialog.ApplicationName = _appInfo.ShortName;
        dialog.ApplicationIcon = _appInfo.Version!.IsPreview ? $"{_appInfo.Id}-devel" : _appInfo.Id;
        dialog.DeveloperName = "Nickvision";
        dialog.Version = _appInfo.Version.ToString();
        dialog.ReleaseNotes = _appInfo.HtmlChangelog;
        dialog.DebugInfo = _controller.GetDebugInformation(extraInfo);
        dialog.Comments = _appInfo.Description;
        dialog.LicenseType = Gtk.License.MitX11;
        dialog.Copyright = "© Nickvision 2021-2026";
        dialog.Website = "https://nickvision.org";
        dialog.IssueUrl = _appInfo.IssueTracker!.ToString();
        dialog.SupportUrl = _appInfo.DiscussionsForum!.ToString();
        dialog.AddLink(_translationService._("GitHub Repo"), _appInfo.SourceRepository!.ToString());
        foreach (var pair in _appInfo.ExtraLinks)
        {
            dialog.AddLink(pair.Key, pair.Value.ToString());
        }
        dialog.Developers = _appInfo.Developers.Select(x => $"{x.Key} {x.Value}").ToArray();
        dialog.Designers = _appInfo.Designers.Select(x => $"{x.Key} {x.Value}").ToArray();
        dialog.Artists = _appInfo.Artists.Select(x => $"{x.Key} {x.Value}").ToArray();
        if (!string.IsNullOrEmpty(_appInfo.TranslationCredits) && _appInfo.TranslationCredits != "translation-credits")
        {
            dialog.TranslatorCredits = _appInfo.TranslationCredits;
        }
        dialog.Present(this);
    }
}
