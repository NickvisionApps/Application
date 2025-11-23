using Nickvision.Application.Shared.Controllers;

namespace Nickvision.Application.GNOME.Views;

public class MainWindow : Adw.ApplicationWindow
{
    private MainWindowController _controller;
    private Gtk.Builder _builder;

    public MainWindow(MainWindowController controller, Gtk.Builder builder) : base(new Adw.Internal.ApplicationWindowHandle(builder.GetPointer("root"), false))
    {
        _controller = controller;
        _builder = builder;
    }
}
