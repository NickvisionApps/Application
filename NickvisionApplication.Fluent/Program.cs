using Avalonia;
using DesktopNotifications.Avalonia;
using System;

namespace NickvisionApplication.Fluent;

class Program
{
    [STAThread]
    public static void Main(string[] args) => BuildAvaloniaApp().StartWithClassicDesktopLifetime(args);

    public static AppBuilder BuildAvaloniaApp() => AppBuilder.Configure<App>().UsePlatformDetect().SetupDesktopNotifications().LogToTrace();
}
