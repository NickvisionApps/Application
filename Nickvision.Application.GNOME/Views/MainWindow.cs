using Nickvision.Application.Shared.Controllers;
using Nickvision.Application.Shared.Events;
using Nickvision.Desktop.Application;
using Nickvision.Desktop.Notifications;
using System;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;

namespace Nickvision.Application.GNOME.Views;

public partial class MainWindow : Adw.ApplicationWindow
{
    private MainWindowController _controller;
    private Gtk.Builder _builder;
    private Gtk.DropTarget _dropTarget;
    private Gio.SimpleAction _actQuit;
    private Gio.SimpleAction _actOpenFolder;
    private Gio.SimpleAction _actCloseFolder;
    private Gio.SimpleAction _actPreferences;
    private Gio.SimpleAction _actKeyboardShortcuts;
    private Gio.SimpleAction _actAbout;

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

    public MainWindow(MainWindowController controller, Adw.Application application, Gtk.Builder builder) : base(new Adw.Internal.ApplicationWindowHandle(builder.GetPointer("root"), false))
    {
        Application = application;
        _controller = controller;
        _builder = builder;
        _builder.Connect(this);
        // Window
        Title = _controller.AppInfo.ShortName;
        IconName = _controller.AppInfo.Id;
        if (_controller.AppInfo.Version!.IsPreview)
        {
            AddCssClass("devel");
        }
        _windowTitle!.Title = _controller.AppInfo.ShortName;
        _pageGreeting!.Title = _controller.Greeting;
        // Events
        OnCloseRequest += Window_OnCloseRequest;
        _controller.AppNotificationSent += (sender, args) => GLib.Functions.IdleAdd(200, () =>
        {
            Controller_AppNotificationSent(sender, args);
            return false;
        });
        _controller.FolderChanged += Controller_FolderChanged;
        // Drop target
        _dropTarget = Gtk.DropTarget.New(Gio.FileHelper.GetGType(), Gdk.DragAction.Copy);
        _dropTarget.OnDrop += Window_OnDrop;
        AddController(_dropTarget);
        // Quit action
        _actQuit = Gio.SimpleAction.New("quit", null);
        _actQuit.OnActivate += Quit;
        AddAction(_actQuit);
        Application!.SetAccelsForAction("win.quit", ["<Ctrl>q"]);
        // Open folder action
        _actOpenFolder = Gio.SimpleAction.New("openFolder", null);
        _actOpenFolder.OnActivate += OpenFolder;
        AddAction(_actOpenFolder);
        Application!.SetAccelsForAction("win.openFolder", ["<Ctrl>o"]);
        // Close folder action
        _actCloseFolder = Gio.SimpleAction.New("closeFolder", null);
        _actCloseFolder.OnActivate += CloseFolder;
        AddAction(_actCloseFolder);
        Application!.SetAccelsForAction("win.closeFolder", ["<Ctrl>w"]);
        // Preferences action
        _actPreferences = Gio.SimpleAction.New("preferences", null);
        _actPreferences.OnActivate += Preferences;
        AddAction(_actPreferences);
        Application!.SetAccelsForAction("win.preferences", ["<Ctrl>."]);
        // Keyboard shortcuts action
        _actKeyboardShortcuts = Gio.SimpleAction.New("keyboardShortcuts", null);
        _actKeyboardShortcuts.OnActivate += KeyboardShortcuts;
        AddAction(_actKeyboardShortcuts);
        Application!.SetAccelsForAction("win.keyboardShortcuts", ["<Ctrl>question"]);
        // About action
        _actAbout = Gio.SimpleAction.New("about", null);
        _actAbout.OnActivate += About;
        AddAction(_actAbout);
        Application!.SetAccelsForAction("win.about", ["F1"]);
    }

    public new void Present()
    {
        base.Present();
        var windowGeometry = _controller.WindowGeometry;
        if (windowGeometry.IsMaximized)
        {
            Maximize();
        }
        else
        {
            SetDefaultSize(windowGeometry.Width, windowGeometry.Height);
        }
    }

    private bool Window_OnCloseRequest(Gtk.Window sender, EventArgs args)
    {
        if (!_controller.CanShutdown)
        {
            return true;
        }
        GetDefaultSize(out int width, out int height);
        _controller.WindowGeometry = new WindowGeometry(width, height, IsMaximized());
        _controller.Dispose();
        Destroy();
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
        if(args.Notification.Action == "close")
        {
            toast.ButtonLabel = _controller.Translator._("Close");
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
        _pageFiles!.Description = _controller.Translator._n("There is {0} file in the folder.", "There are {0} files in the folder", args.Files.Count, args.Files.Count);
    }

    private void Quit(Gio.SimpleAction sender, Gio.SimpleAction.ActivateSignalArgs args)
    {
        if (!Window_OnCloseRequest(this, new EventArgs()))
        {
            Application!.Quit();
        }
    }

    private async void OpenFolder(Gio.SimpleAction sender, Gio.SimpleAction.ActivateSignalArgs args)
    {
        var folderDialog = Gtk.FileDialog.New();
        folderDialog.Title = _controller.Translator._("Open Folder");
        var file = await folderDialog.SelectFolderAsync(this);
        if(file is not null)
        {
            _controller.OpenFolder(file.GetPath()!);
        }
    }

    private void CloseFolder(Gio.SimpleAction sender, Gio.SimpleAction.ActivateSignalArgs args) => _controller.CloseFolder();

    private void Preferences(Gio.SimpleAction sender, Gio.SimpleAction.ActivateSignalArgs args)
    {

    }

    private void KeyboardShortcuts(Gio.SimpleAction sender, Gio.SimpleAction.ActivateSignalArgs args)
    {
        var builder = Gtk.Builder.NewFromFile(Path.Combine(Desktop.System.Environment.ExecutingDirectory, "ui", "ShortcutsDialog.ui"));
        var dialog = new Adw.ShortcutsDialog(new Adw.Internal.ShortcutsDialogHandle(builder.GetPointer("root"), false));
        dialog.Present(this);
    }

    private void About(Gio.SimpleAction sender, Gio.SimpleAction.ActivateSignalArgs args)
    {
        var extraInfo = string.Empty;
        extraInfo += $"GTK {Gtk.Functions.GetMajorVersion()}.{Gtk.Functions.GetMinorVersion()}.{Gtk.Functions.GetMicroVersion()}\n";
        extraInfo += $"libadwaita {Adw.Functions.GetMajorVersion()}.{Adw.Functions.GetMinorVersion()}.{Adw.Functions.GetMicroVersion()}";
        var dialog = Adw.AboutDialog.New();
        dialog.ApplicationName = _controller.AppInfo.ShortName;
        dialog.ApplicationIcon = _controller.AppInfo.Version!.IsPreview ? $"{_controller.AppInfo.Id}-devel" : _controller.AppInfo.Id;
        dialog.DeveloperName = "Nickvision";
        dialog.Version = _controller.AppInfo.Version.ToString();
        dialog.ReleaseNotes = _controller.AppInfo.HtmlChangelog;
        dialog.DebugInfo = _controller.GetDebugInformation(extraInfo);
        dialog.Comments = _controller.AppInfo.Description;
        dialog.LicenseType = Gtk.License.MitX11;
        dialog.Copyright = "© Nickvision 2021-2025";
        dialog.Website = "https://nickvision.org";
        dialog.IssueUrl = _controller.AppInfo.IssueTracker!.ToString();
        dialog.SupportUrl = _controller.AppInfo.DiscussionsForum!.ToString();
        dialog.AddLink(_controller.Translator._("GitHub Repo"), _controller.AppInfo.SourceRepository!.ToString());
        foreach(var pair in _controller.AppInfo.ExtraLinks)
        {
            dialog.AddLink(pair.Key, pair.Value.ToString());
        }
        dialog.Developers = _controller.AppInfo.Developers.Select(x => $"{x.Key} {x.Value}").ToArray();
        dialog.Designers = _controller.AppInfo.Designers.Select(x => $"{x.Key} {x.Value}").ToArray();
        dialog.Artists = _controller.AppInfo.Designers.Select(x => $"{x.Key} {x.Value}").ToArray();
        if (!string.IsNullOrEmpty(_controller.AppInfo.TranslationCredits) && _controller.AppInfo.TranslationCredits != "translation-credits")
        {
            dialog.TranslatorCredits = _controller.AppInfo.TranslationCredits;
        }   
        dialog.Present(this);
    }
}
