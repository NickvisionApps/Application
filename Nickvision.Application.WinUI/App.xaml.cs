using Microsoft.Extensions.DependencyInjection;
using Microsoft.Extensions.Logging;
using Microsoft.UI.Xaml;
using Microsoft.Windows.AppNotifications;
using Nickvision.Application.WinUI.Views;
using System;
using System.Diagnostics;
using System.IO;
using Windows.System;

namespace Nickvision.Application.WinUI;

public partial class App : Microsoft.UI.Xaml.Application
{
    private readonly IServiceProvider _serviceProvider;
    private Window? _window;

    public App(IServiceProvider serviceProvider)
    {
        InitializeComponent();
        _serviceProvider = serviceProvider;
        AppNotificationManager.Default.NotificationInvoked += App_NotificationInvoked;
        AppNotificationManager.Default.Register();
        AppDomain.CurrentDomain.ProcessExit += async (_, _) =>
        {
            await AppNotificationManager.Default.RemoveAllAsync();
            AppNotificationManager.Default.UnregisterAll();
        };
        UnhandledException += (_, e) =>
        {
            _serviceProvider.GetRequiredService<ILogger<App>>().LogError(e.Exception, $"An unhandled exception occurred: {e.Message}");
        };
    }

    protected override void OnLaunched(LaunchActivatedEventArgs args)
    {
        if (_window is null)
        {
            _window = _serviceProvider.GetRequiredService<MainWindow>();
        }
        _window.Activate();
    }

    private async void App_NotificationInvoked(AppNotificationManager sender, AppNotificationActivatedEventArgs args)
    {
        if (args.Arguments.ContainsKey("action") && args.Arguments["action"] == "OpenInExplorer" && args.Arguments.ContainsKey("param") && Directory.Exists(args.Arguments["param"]))
        {
            try
            {
                using var _ = Process.Start(new ProcessStartInfo()
                {
                    FileName = args.Arguments["param"],
                    UseShellExecute = true
                });
            }
            catch
            {
                await Launcher.LaunchFolderPathAsync(args.Arguments["param"]);
            }
        }
    }
}
