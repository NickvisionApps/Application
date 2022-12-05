namespace NickvisionApplication.GNOME;

public class Program
{
    public static int Main(string[] args)
    {
        var application = Adw.Application.New("org.nickvision.application", Gio.ApplicationFlags.FlagsNone);
        application.OnActivate += (sender, e) =>
        {
            var window = Adw.ApplicationWindow.New(application);
            window.Show();
        };
        return application.Run();
    }
}
