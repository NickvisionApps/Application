using NickvisionApplication.Shared.Controllers;
using NickvisionApplication.Shared.Events;
using System;
using System.Collections.Generic;

namespace NickvisionApplication.GNOME.Views;

/// <summary>
/// The MainWindow for the application
/// </summary>
public class MainWindow : Adw.ApplicationWindow
{
    private readonly MainWindowController _controller;
    private readonly Gtk.Box _mainBox;
    private readonly Adw.HeaderBar _headerBar;
    private readonly Adw.WindowTitle _windowTitle;
    private readonly Gtk.Button _btnOpenFolder;
    private readonly Gtk.Button _btnCloseFolder;
    private readonly Gtk.MenuButton _btnMenuHelp;
    private readonly Adw.ToastOverlay _toastOverlay;
    private readonly Adw.ViewStack _viewStack;
    private readonly Adw.StatusPage _pageNoFolder;

    /// <summary>
    /// Constructs a MainWindow
    /// </summary>
    /// <param name="controller">The MainWindowController</param>
    public MainWindow(MainWindowController controller, Gtk.Application application)
    {
        //Window Settings
        _controller = controller;
        New();
        SetDefaultSize(800, 600);
        if(_controller.IsDevVersion)
        {
            AddCssClass("devel");
        }
        //Main Box
        _mainBox = Gtk.Box.New(Gtk.Orientation.Vertical, 0);
        //Header Bar
        _headerBar = Adw.HeaderBar.New();
        _windowTitle = Adw.WindowTitle.New(_controller.AppInfo.ShortName, _controller.FolderPath);
        _headerBar.SetTitleWidget(_windowTitle);
        _mainBox.Append(_headerBar);
        //Open Folder Button
        _btnOpenFolder = Gtk.Button.New();
        var btnOpenFolderContent = Adw.ButtonContent.New();
        btnOpenFolderContent.SetLabel("Open");
        btnOpenFolderContent.SetIconName("folder-open-symbolic");
        _btnOpenFolder.SetChild(btnOpenFolderContent);
        _btnOpenFolder.SetTooltipText("Open Folder (Ctrl+O)");
        _btnOpenFolder.SetActionName("win.openFolder");
        _headerBar.PackStart(_btnOpenFolder);
        //Close Folder Button
        _btnCloseFolder = Gtk.Button.New();
        _btnCloseFolder.SetIconName("window-close-symbolic");
        _btnCloseFolder.SetVisible(false);
        _btnCloseFolder.SetActionName("win.closeFolder");
        _headerBar.PackStart(_btnCloseFolder);
        //Menu Help Button
        _btnMenuHelp = Gtk.MenuButton.New();
        var menuHelp = Gio.Menu.New();
        menuHelp.Append("Preferences", "win.preferences");
        menuHelp.Append("Keyboard Shortcuts", "win.keyboardShortcuts");
        menuHelp.Append($"About {_controller.AppInfo.ShortName}", "win.about");
        _btnMenuHelp.SetDirection(Gtk.ArrowType.None);
        _btnMenuHelp.SetMenuModel(menuHelp);
        _btnMenuHelp.SetTooltipText("Main Menu");
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
        _pageNoFolder.SetIconName("folder-symbolic");
        _pageNoFolder.SetTitle("No Folder Opened");
        _pageNoFolder.SetDescription("Open a folder (or drag one into the app) to get started.");
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
        //About Action
        var actAbout = Gio.SimpleAction.New("about", null);
        actAbout.OnActivate += About;
        AddAction(actAbout);
    }

    /// <summary>
    /// Occurs when a notification is sent from the controller
    /// </summary>
    /// <param name="sender">object?</param>
    /// <param name="e">NotificationSentEventArgs</param>
    private void NotificationSent(object? sender, NotificationSentEventArgs e) => _toastOverlay.AddToast(Adw.Toast.New(e.Message));

    /// <summary>
    /// Occurs when the folder is changed
    /// </summary>
    /// <param name="sender">object?</param>
    /// <param name="e">EventArgs</param>
    private void FolderChanged(object? sender, EventArgs e)
    {
        _windowTitle.SetSubtitle(_controller.FolderPath);
        _btnCloseFolder.SetVisible(true);
        _viewStack.SetVisibleChildName(_controller.IsFolderOpened ? "Folder" : "NoFolder");
    }

    /// <summary>
    /// Occurs when the open folder action is triggered
    /// </summary>
    /// <param name="sender">Gio.SimpleAction</param>
    /// <param name="e">EventArgs</param>
    private void OpenFolder(Gio.SimpleAction sender, EventArgs e)
    {
        var openFolderDialog = Gtk.FileChooserNative.New("Open Folder", this, Gtk.FileChooserAction.SelectFolder, "Open", "Cancel");
        openFolderDialog.SetModal(true);
        openFolderDialog.OnResponse += (sender, e) =>
        {
            if (e.ResponseId == (int)Gtk.ResponseType.Accept)
            {
                _controller.OpenFolder(openFolderDialog.GetFile().GetPath() ?? "");
            }
        };
        openFolderDialog.Show();
    }

    /// <summary>
    /// Occurs when the close folder action is triggered
    /// </summary>
    /// <param name="sender">Gio.SimpleAction</param>
    /// <param name="e">EventArgs</param>
    private void CloseFolder(Gio.SimpleAction sender, EventArgs e) => _controller.CloseFolder();

    /// <summary>
    /// Occurs when the preferences action is triggered
    /// </summary>
    /// <param name="sender">Gio.SimpleAction</param>
    /// <param name="e">EventArgs</param>
    private void Preferences(Gio.SimpleAction sender, EventArgs e)
    {
        
    }

    /// <summary>
    /// Occurs when the keyboard shortcuts action is triggered
    /// </summary>
    /// <param name="sender">Gio.SimpleAction</param>
    /// <param name="e">EventArgs</param>
    private void KeyboardShortcuts(Gio.SimpleAction sender, EventArgs e)
    {
        
    }

    /// <summary>
    /// Occurs when the about action is triggered
    /// </summary>
    /// <param name="sender">Gio.SimpleAction</param>
    /// <param name="e">EventArgs</param>
    private void About(Gio.SimpleAction sender, EventArgs e)
    {
        
    }
}
