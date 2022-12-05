using NickvisionApplication.Shared.Controllers;
using NickvisionApplication.Shared.Events;

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
    private readonly Adw.ToastOverlay _toastOverlay;

    /// <summary>
    /// Constructs a MainWindow
    /// </summary>
    /// <param name="controller">The MainWindowController</param>
    public MainWindow(MainWindowController controller)
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
        _windowTitle = Adw.WindowTitle.New(_controller.AppInfo.ShortName, "");
        _headerBar.SetTitleWidget(_windowTitle);
        _mainBox.Append(_headerBar);
        //Toast Overlay
        _toastOverlay = Adw.ToastOverlay.New();
        _toastOverlay.SetHexpand(true);
        _toastOverlay.SetVexpand(true);
        _mainBox.Append(_toastOverlay);
        //Layout
        SetContent(_mainBox);
        //Register Events 
        _controller.NotificationSent += NotificationSent;
    }

    /// <summary>
    /// Occurs when a notification is sent from the controller
    /// </summary>
    /// <param name="sender">object?</param>
    /// <param name="e">NotificationSentEventArgs</param>
    private void NotificationSent(object? sender, NotificationSentEventArgs e) => _toastOverlay.AddToast(Adw.Toast.New(e.Message));
}
