using NickvisionApplication.Shared.Controllers;

namespace NickvisionApplication.GNOME.Views;

public class MainWindow : Adw.ApplicationWindow
{
    private readonly MainWindowController _controller;
    private readonly Gtk.Box _mainBox;
    private readonly Adw.HeaderBar _headerBar;
    private readonly Adw.WindowTitle _windowTitle;

    public MainWindow(MainWindowController controller)
    {
        //Window Settings
        _controller = controller;
        SetDefaultSize(800, 600);
        //Main Box
        _mainBox = Gtk.Box.New(Gtk.Orientation.Vertical, 0);
        //Header Bar
        _headerBar = Adw.HeaderBar.New();
        _windowTitle = Adw.WindowTitle.New(_controller.AppInfo.ShortName, "");
        _headerBar.SetTitleWidget(_windowTitle);
        _mainBox.Append(_headerBar);
        //Layout
        SetContent(_mainBox);
    }
}
