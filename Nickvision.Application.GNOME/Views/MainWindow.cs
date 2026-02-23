using Microsoft.Extensions.DependencyInjection;
using Microsoft.Extensions.Hosting;
using Nickvision.Application.Shared.Controllers;
using Nickvision.Application.Shared.Events;
using Nickvision.Application.Shared.Models;
using Nickvision.Desktop.Application;
using Nickvision.Desktop.Globalization;
using Nickvision.Desktop.GNOME.Controls;
using Nickvision.Desktop.GNOME.Helpers;
using Nickvision.Desktop.Notifications;
using System;
using System.Linq;

namespace Nickvision.Application.GNOME.Views;

public class MainWindow : Adw.ApplicationWindow
{
    private readonly IServiceProvider _serviceProvider;
    private readonly MainWindowController _controller;
    private readonly AppInfo _appInfo;
    private readonly ITranslationService _translationService;
    private readonly Gtk.Builder _builder;

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

    public MainWindow(IServiceProvider serviceProvider, MainWindowController controller, AppInfo appInfo, ITranslationService translationService, IGtkBuilderFactory builderFactory) : this(serviceProvider, controller, appInfo, translationService, builderFactory.Create("MainWindow"))
    {

    }

    private MainWindow(IServiceProvider serviceProvider, MainWindowController controller, AppInfo appInfo, ITranslationService translationService, Gtk.Builder builder) : base(new Adw.Internal.ApplicationWindowHandle(builder.GetPointer("root"), false))
    {
        var application = serviceProvider.GetRequiredService<Adw.Application>();
        _serviceProvider = serviceProvider;
        _controller = controller;
        _appInfo = appInfo;
        _translationService = translationService;
        _builder = builder;
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
        OnCloseRequest += Window_OnCloseRequest;
        _controller.AppNotificationSent += (sender, args) => GLib.Functions.IdleAdd(0, () =>
        {
            Controller_AppNotificationSent(sender, args);
            return false;
        });
        _controller.FolderChanged += Controller_FolderChanged;
        // Drop target
        var dropTarget = Gtk.DropTarget.New(Gio.FileHelper.GetGType(), Gdk.DragAction.Copy);
        dropTarget.OnDrop += Window_OnDrop;
        AddController(dropTarget);
        // Quit action
        var actQuit = Gio.SimpleAction.New("quit", null);
        actQuit.OnActivate += Quit;
        AddAction(actQuit);
        application.SetAccelsForAction("win.quit", ["<Ctrl>q"]);
        // Open folder action
        var actOpenFolder = Gio.SimpleAction.New("openFolder", null);
        actOpenFolder.OnActivate += OpenFolder;
        AddAction(actOpenFolder);
        application.SetAccelsForAction("win.openFolder", ["<Ctrl>o"]);
        // Close folder action
        var actCloseFolder = Gio.SimpleAction.New("closeFolder", null);
        actCloseFolder.OnActivate += CloseFolder;
        AddAction(actCloseFolder);
        application.SetAccelsForAction("win.closeFolder", ["<Ctrl>w"]);
        // Preferences action
        var actPreferences = Gio.SimpleAction.New("preferences", null);
        actPreferences.OnActivate += Preferences;
        AddAction(actPreferences);
        application.SetAccelsForAction("win.preferences", ["<Ctrl>period"]);
        // Keyboard shortcuts action
        var actKeyboardShortcuts = Gio.SimpleAction.New("keyboardShortcuts", null);
        actKeyboardShortcuts.OnActivate += KeyboardShortcuts;
        AddAction(actKeyboardShortcuts);
        application.SetAccelsForAction("win.keyboardShortcuts", ["<Ctrl>question"]);
        // About action
        var actAbout = Gio.SimpleAction.New("about", null);
        actAbout.OnActivate += About;
        AddAction(actAbout);
        application.SetAccelsForAction("win.about", ["F1"]);
    }

    public new void Present()
    {
        base.Present();
        this.WindowGeometry = _controller.WindowGeometry;
    }

    private bool Window_OnCloseRequest(Gtk.Window sender, EventArgs args)
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

    private bool Window_OnDrop(Gtk.DropTarget sender, Gtk.DropTarget.DropSignalArgs args)
    {
        var file = new Gio.FileHelper(args.Value.GetObject()!.Handle);
        var path = file.GetPath();
        if (string.IsNullOrEmpty(path))
        {
            return false;
        }
        _controller.OpenFolder(path);
        return true;
    }

    private void Controller_AppNotificationSent(object? sender, AppNotificationSentEventArgs args)
    {
        var toast = Adw.Toast.New(args.Notification.Message);
        if (args.Notification.Action == "close")
        {
            toast.ButtonLabel = _translationService._("Close");
            toast.OnButtonClicked += (_, _) => _controller.CloseFolder();
        }
        _toastOverlay!.AddToast(toast);
    }

    private void Controller_FolderChanged(object? sender, FolderChangedEventArgs args)
    {
        _windowTitle!.Subtitle = args.IsOpen ? args.Path : string.Empty;
        _btnOpenFolder!.Visible = args.IsOpen;
        _btnCloseFolder!.Visible = args.IsOpen;
        _viewStack!.VisibleChildName = args.IsOpen ? "Folder" : "NoFolder";
        _pageFiles!.Description = _translationService._n("There is {0} file in the folder.", "There are {0} files in the folder", args.Files.Count, args.Files.Count);
    }

    private void Quit(Gio.SimpleAction sender, Gio.SimpleAction.ActivateSignalArgs args) => Window_OnCloseRequest(this, new EventArgs());

    private async void OpenFolder(Gio.SimpleAction sender, Gio.SimpleAction.ActivateSignalArgs args)
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

    private void CloseFolder(Gio.SimpleAction sender, Gio.SimpleAction.ActivateSignalArgs args) => _controller.CloseFolder();

    private void Preferences(Gio.SimpleAction sender, Gio.SimpleAction.ActivateSignalArgs args) => _serviceProvider.GetRequiredService<PreferencesDialog>().Present(this);

    private void KeyboardShortcuts(Gio.SimpleAction sender, Gio.SimpleAction.ActivateSignalArgs args) => _serviceProvider.GetRequiredService<ShortcutsDialog>().Present(this);

    private void About(Gio.SimpleAction sender, Gio.SimpleAction.ActivateSignalArgs args)
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
