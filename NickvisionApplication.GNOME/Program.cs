namespace NickvisionApplication.GNOME;

public class Program
{
    public static int Main(string[] args)
    {
        Adw.Module.Initialize();
        var application = Adw.Application.New("org.nickvision.application", Gio.ApplicationFlags.FlagsNone);
        application.OnActivate += (sender, e) =>
        {
            var window = Adw.ApplicationWindow.New(application);
            window.SetDefaultSize(800, 600);
            var box = Gtk.Box.New(Gtk.Orientation.Vertical, 0);
            var headerBar = Adw.HeaderBar.New();
            var button = Gtk.Button.New();
            var buttonContent = Adw.ButtonContent.New();
            buttonContent.SetLabel("Open");
            buttonContent.SetIconName("folder-symbolic");
            button.SetChild(buttonContent);
            headerBar.PackStart(button);
            box.Append(headerBar);
            window.SetContent(box);
            window.Show();
        };
        return application.Run();
    }
}
