using Nickvision.Application.Shared.Controllers;

namespace Nickvision.Application.Gtk.Views;

public class MainWindow : Adw.ApplicationWindow
{
    private MainWindowController _controller;

    public MainWindow(MainWindowController controller)
    {
        _controller = controller;
    }
}
