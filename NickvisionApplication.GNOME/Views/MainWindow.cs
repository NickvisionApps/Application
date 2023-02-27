using NickvisionApplication.GNOME.Controls;
using NickvisionApplication.GNOME.Helpers;
using NickvisionApplication.Shared.Controllers;
using NickvisionApplication.Shared.Events;
using System;
using System.IO;
using System.Runtime.InteropServices;

namespace NickvisionApplication.GNOME.Views;

/// <summary>
/// The MainWindow for the application
/// </summary>
public partial class MainWindow : Adw.ApplicationWindow
{
    [LibraryImport("libadwaita-1.so.0", StringMarshalling = StringMarshalling.Utf8)]
    private static partial string g_file_get_path(nint file);

    private readonly MainWindowController _controller;
    private readonly Adw.Application _application;

    [Gtk.Connect] private readonly Gtk.Box _mainBox;
    [Gtk.Connect] private readonly Adw.WindowTitle _title;
    [Gtk.Connect] private readonly Gtk.Button _closeFolderButton;
    [Gtk.Connect] private readonly Adw.ToastOverlay _toastOverlay;
    [Gtk.Connect] private readonly Adw.ViewStack _viewStack;
    private readonly Gtk.DropTarget _dropTarget;

    /// <summary>
    /// Constructs a MainWindow
    /// </summary>
    /// <param name="controller">The MainWindowController</param>
    /// <param name="application">The Adw.Application</param>
    public MainWindow(MainWindowController controller, Adw.Application application)
    {
        //Window Settings
        _controller = controller;
        _application = application;
        SetDefaultSize(800, 600);
        SetTitle(_controller.AppInfo.ShortName);
        if (_controller.IsDevVersion)
        {
            AddCssClass("devel");
        }
        //Build UI
        var builder = Builder.FromFile("window.ui", _controller.Localizer, (s) => s == "About" ? string.Format(_controller.Localizer[s], _controller.AppInfo.ShortName) : _controller.Localizer[s]);
        builder.Connect(this);
        _title.SetTitle(_controller.AppInfo.ShortName);
        _title.SetSubtitle(_controller.FolderPath == "No Folder Opened" ? _controller.Localizer["NoFolderOpened"] : _controller.FolderPath);
        var greeting = (Adw.StatusPage)builder.GetObject("_greeting");
        greeting.SetIconName(controller.ShowSun ? "sun-outline-symbolic" : "moon-outline-symbolic");
        greeting.SetTitle(_controller.Greeting);
        SetContent(_mainBox);
        //Register Events 
        _controller.NotificationSent += NotificationSent;
        _controller.FolderChanged += FolderChanged;
        //Open Folder Action
        var actOpenFolder = Gio.SimpleAction.New("openFolder", null);
        actOpenFolder.OnActivate += OpenFolder;
        AddAction(actOpenFolder);
        application.SetAccelsForAction("win.openFolder", new string[] { "<Ctrl>O" });
        //Close Folder Action
        var actCloseFolder = Gio.SimpleAction.New("closeFolder", null);
        actCloseFolder.OnActivate += CloseFolder;
        AddAction(actCloseFolder);
        application.SetAccelsForAction("win.closeFolder", new string[] { "<Ctrl>W" });
        //Preferences Action
        var actPreferences = Gio.SimpleAction.New("preferences", null);
        actPreferences.OnActivate += Preferences;
        AddAction(actPreferences);
        application.SetAccelsForAction("win.preferences", new string[] { "<Ctrl>comma" });
        //Keyboard Shortcuts Action
        var actKeyboardShortcuts = Gio.SimpleAction.New("keyboardShortcuts", null);
        actKeyboardShortcuts.OnActivate += KeyboardShortcuts;
        AddAction(actKeyboardShortcuts);
        application.SetAccelsForAction("win.keyboardShortcuts", new string[] { "<Ctrl>question" });
        //Quit Action
        var actQuit = Gio.SimpleAction.New("quit", null);
        actQuit.OnActivate += Quit;
        AddAction(actQuit);
        application.SetAccelsForAction("win.quit", new string[] { "<Ctrl>q" });
        //About Action
        var actAbout = Gio.SimpleAction.New("about", null);
        actAbout.OnActivate += About;
        AddAction(actAbout);
        application.SetAccelsForAction("win.about", new string[] { "F1" });
        //Drop Target
        _dropTarget = Gtk.DropTarget.New(Gio.FileHelper.GetGType(), Gdk.DragAction.Copy);
        _dropTarget.OnDrop += OnDrop;
        AddController(_dropTarget);
    }

    /// <summary>
    /// Starts the MainWindow
    /// </summary>
    public void Start()
    {
        _application.AddWindow(this);
        Show();
    }

    /// <summary>
    /// Occurs when a notification is sent from the controller
    /// </summary>
    /// <param name="sender">object?</param>
    /// <param name="e">NotificationSentEventArgs</param>
    private void NotificationSent(object? sender, NotificationSentEventArgs e) => _toastOverlay.AddToast(Adw.Toast.New(e.Message));

    /// <summary>
    /// Occurs when something is dropped onto the window
    /// </summary>
    /// <param name="sender">Gtk.DropTarget</param>
    /// <param name="e">Gtk.DropTarget.DropSignalArgs</param>
    private bool OnDrop(Gtk.DropTarget sender, Gtk.DropTarget.DropSignalArgs e)
    {
        var obj = e.Value.GetObject();
        if (obj != null)
        {
            var path = g_file_get_path(obj.Handle);
            if (Directory.Exists(path))
            {
                _controller.OpenFolder(path);
                return true;
            }
        }
        return false;
    }

    /// <summary>
    /// Occurs when the folder is changed
    /// </summary>
    /// <param name="sender">object?</param>
    /// <param name="e">EventArgs</param>
    private void FolderChanged(object? sender, EventArgs e)
    {
        _title.SetSubtitle(_controller.FolderPath);
        _closeFolderButton.SetVisible(_controller.IsFolderOpened);
        _viewStack.SetVisibleChildName(_controller.IsFolderOpened ? "Folder" : "NoFolder");
    }

    /// <summary>
    /// Occurs when the open folder action is triggered
    /// </summary>
    /// <param name="sender">Gio.SimpleAction</param>
    /// <param name="e">EventArgs</param>
    private void OpenFolder(Gio.SimpleAction sender, EventArgs e)
    {
        var openFolderDialog = Gtk.FileChooserNative.New(_controller.Localizer["OpenFolder"], this, Gtk.FileChooserAction.SelectFolder, _controller.Localizer["Open"], _controller.Localizer["Cancel"]);
        openFolderDialog.SetModal(true);
        openFolderDialog.OnResponse += (sender, e) =>
        {
            if (e.ResponseId == (int)Gtk.ResponseType.Accept)
            {
                var path = openFolderDialog.GetFile()!.GetPath();
                _controller.OpenFolder(path ?? "");
            }
        };
        openFolderDialog.Show();
    }

    /// <summary>
    /// Occurs when the close folder action is triggered
    /// </summary>
    /// <param name="sender">Gio.SimpleAction</param>
    /// <param name="e">EventArgs</param>
    private void CloseFolder(Gio.SimpleAction sender, EventArgs e)
    {
        var dialog = new MessageDialog(this, _controller.Localizer["CloseFolderDialog", "Title"], _controller.Localizer["CloseFolderDialog", "Description"], _controller.Localizer["Cancel"], _controller.Localizer["Close"]);
        dialog.Show();
        dialog.OnResponse += (sender, e) =>
        {
            if (dialog.Response == MessageDialogResponse.Destructive)
            {
                _controller.CloseFolder();
            }
            dialog.Destroy();
        };
    }

    /// <summary>
    /// Occurs when the preferences action is triggered
    /// </summary>
    /// <param name="sender">Gio.SimpleAction</param>
    /// <param name="e">EventArgs</param>
    private void Preferences(Gio.SimpleAction sender, EventArgs e)
    {
        var preferencesDialog = new PreferencesDialog(_controller.PreferencesViewController, _application, this);
        preferencesDialog.Show();
    }

    /// <summary>
    /// Occurs when the keyboard shortcuts action is triggered
    /// </summary>
    /// <param name="sender">Gio.SimpleAction</param>
    /// <param name="e">EventArgs</param>
    private void KeyboardShortcuts(Gio.SimpleAction sender, EventArgs e)
    {
        var builder = Builder.FromFile("shortcuts_dialog.ui", _controller.Localizer, (s) => s == "About" ? string.Format(_controller.Localizer[s], _controller.AppInfo.ShortName) : _controller.Localizer[s]);
        var shortcutsWindow = (Gtk.ShortcutsWindow)builder.GetObject("_shortcuts");
        shortcutsWindow.SetTransientFor(this);
        shortcutsWindow.Show();
    }

    /// <summary>
    /// Occurs when quit action is triggered
    /// </summary>
    /// <param name="sender">Gio.SimpleAction</param>
    /// <param name="e">EventArgs</param>
    private void Quit(Gio.SimpleAction sender, EventArgs e) => _application.Quit();

    /// <summary>
    /// Occurs when the about action is triggered
    /// </summary>
    /// <param name="sender">Gio.SimpleAction</param>
    /// <param name="e">EventArgs</param>
    private void About(Gio.SimpleAction sender, EventArgs e)
    {
        var dialog = Adw.AboutWindow.New();
        dialog.SetTransientFor(this);
        dialog.SetApplicationName(_controller.AppInfo.ShortName);
        dialog.SetApplicationIcon(_controller.AppInfo.ID + (_controller.AppInfo.GetIsDevelVersion() ? "-devel" : ""));
        dialog.SetVersion(_controller.AppInfo.Version);
        dialog.SetComments(_controller.AppInfo.Description);
        dialog.SetDeveloperName("Nickvision");
        dialog.SetLicenseType(Gtk.License.MitX11);
        dialog.SetCopyright("© Nickvision 2021-2023");
        dialog.SetWebsite(_controller.AppInfo.GitHubRepo.ToString());
        dialog.SetIssueUrl(_controller.AppInfo.IssueTracker.ToString());
        dialog.SetSupportUrl(_controller.AppInfo.SupportUrl.ToString());
        dialog.SetDevelopers(_controller.Localizer["Developers", "Credits"].Split(Environment.NewLine));
        dialog.SetDesigners(_controller.Localizer["Designers", "Credits"].Split(Environment.NewLine));
        dialog.SetArtists(_controller.Localizer["Artists", "Credits"].Split(Environment.NewLine));
        dialog.SetTranslatorCredits((string.IsNullOrEmpty(_controller.Localizer["Translators", "Credits"]) ? "" : _controller.Localizer["Translators", "Credits"]));
        dialog.SetReleaseNotes(_controller.AppInfo.Changelog);
        dialog.Show();
    }
}
