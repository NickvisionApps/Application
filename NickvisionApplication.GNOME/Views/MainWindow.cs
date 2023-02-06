using NickvisionApplication.GNOME.Controls;
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
    private readonly Gtk.Box _mainBox;
    private readonly Adw.HeaderBar _headerBar;
    private readonly Adw.WindowTitle _windowTitle;
    private readonly Gtk.Button _btnOpenFolder;
    private readonly Gtk.Button _btnCloseFolder;
    private readonly Gtk.MenuButton _btnMenuHelp;
    private readonly Adw.ToastOverlay _toastOverlay;
    private readonly Adw.ViewStack _viewStack;
    private readonly Adw.StatusPage _pageNoFolder;
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
        New();
        SetDefaultSize(800, 600);
        SetTitle(_controller.AppInfo.ShortName);
        if(_controller.IsDevVersion)
        {
            AddCssClass("devel");
        }
        //Main Box
        _mainBox = Gtk.Box.New(Gtk.Orientation.Vertical, 0);
        //Header Bar
        _headerBar = Adw.HeaderBar.New();
        _windowTitle = Adw.WindowTitle.New(_controller.AppInfo.ShortName, _controller.FolderPath == "No Folder Opened" ? _controller.Localizer["NoFolderOpened"] : _controller.FolderPath);
        _headerBar.SetTitleWidget(_windowTitle);
        _mainBox.Append(_headerBar);
        //Open Folder Button
        _btnOpenFolder = Gtk.Button.New();
        var btnOpenFolderContent = Adw.ButtonContent.New();
        btnOpenFolderContent.SetLabel(_controller.Localizer["Open"]);
        btnOpenFolderContent.SetIconName("folder-open-symbolic");
        _btnOpenFolder.SetChild(btnOpenFolderContent);
        _btnOpenFolder.SetTooltipText(_controller.Localizer["OpenFolder", "Tooltip"]);
        _btnOpenFolder.SetActionName("win.openFolder");
        _headerBar.PackStart(_btnOpenFolder);
        //Close Folder Button
        _btnCloseFolder = Gtk.Button.New();
        _btnCloseFolder.SetIconName("window-close-symbolic");
        _btnOpenFolder.SetTooltipText(_controller.Localizer["CloseFolder", "Tooltip"]);
        _btnCloseFolder.SetVisible(false);
        _btnCloseFolder.SetActionName("win.closeFolder");
        _headerBar.PackStart(_btnCloseFolder);
        //Menu Help Button
        _btnMenuHelp = Gtk.MenuButton.New();
        var menuHelp = Gio.Menu.New();
        menuHelp.Append(_controller.Localizer["Preferences"], "win.preferences");
        menuHelp.Append(_controller.Localizer["KeyboardShortcuts"], "win.keyboardShortcuts");
        menuHelp.Append(string.Format(_controller.Localizer["About"], _controller.AppInfo.ShortName), "win.about");
        _btnMenuHelp.SetDirection(Gtk.ArrowType.None);
        _btnMenuHelp.SetMenuModel(menuHelp);
        _btnMenuHelp.SetTooltipText(_controller.Localizer["MainMenu", "GTK"]);
        _headerBar.PackEnd(_btnMenuHelp);
        //Toast Overlay
        _toastOverlay = Adw.ToastOverlay.New();
        _toastOverlay.SetHexpand(true);
        _toastOverlay.SetVexpand(true);
        _mainBox.Append(_toastOverlay);
        //View Stack
        _viewStack = Adw.ViewStack.New();
        _toastOverlay.SetChild(_viewStack);
        //No Folder Page
        _pageNoFolder = Adw.StatusPage.New();
        _pageNoFolder.SetIconName(controller.ShowSun ? "sun-alt-symbolic" : "moon-symbolic");
        _pageNoFolder.SetTitle(_controller.Greeting);
        _pageNoFolder.SetDescription(_controller.Localizer["NoFolderDescription"]);
        _viewStack.AddNamed(_pageNoFolder, "NoFolder");
        //Folder Page
        var pageFolder = Gtk.Box.New(Gtk.Orientation.Vertical, 0);
        _viewStack.AddNamed(pageFolder, "Folder");
        //Layout
        SetContent(_mainBox);
        _viewStack.SetVisibleChildName("NoFolder");
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
        if(obj != null)
        {
            var path = g_file_get_path(obj.Handle);
            if(Directory.Exists(path))
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
        _windowTitle.SetSubtitle(_controller.FolderPath);
        _btnCloseFolder.SetVisible(_controller.IsFolderOpened ? true : false);
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
        var shortcutsDialog = new ShortcutsDialog(_controller.Localizer, _controller.AppInfo.ShortName, this);
        shortcutsDialog.Show();
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
