using NickvisionApplication.GNOME.Views;
using NickvisionApplication.Shared.Controllers;
using NickvisionApplication.Shared.Models;
using System;

namespace NickvisionApplication.GNOME;

public class Program
{
    private readonly Adw.Application _application;

    public static int Main(string[] args) => new Program().Run();

    public Program()
    {
        Adw.Module.Initialize();
        _application = Adw.Application.New("org.nickvision.application", Gio.ApplicationFlags.FlagsNone);
        _application.OnActivate += OnActivate;
    }

    public int Run() => _application.Run();

    private void OnActivate(object sedner, EventArgs e)
    {
        //Controller Setup
        var mainWindowController = new MainWindowController();
        mainWindowController.AppInfo.ID = "org.nickvision.application";
        mainWindowController.AppInfo.Name = "NickvisionApplication";
        mainWindowController.AppInfo.ShortName = "Application";
        mainWindowController.AppInfo.Description = "A template for creating Nickvision applications.";
        mainWindowController.AppInfo.Version = "2022.12.0-next";
        mainWindowController.AppInfo.Changelog = "<ul><li>Initial Release</li></ul>";
        mainWindowController.AppInfo.GitHubRepo = new Uri("https://github.com/nlogozzo/NickvisionApplication");
        mainWindowController.AppInfo.IssueTracker = new Uri("https://github.com/nlogozzo/NickvisionApplication/issues/new");
        mainWindowController.AppInfo.SupportUrl = new Uri("https://github.com/nlogozzo/NickvisionApplication/discussions");
        //Set Adw Theme
        _application.StyleManager!.ColorScheme = mainWindowController.Theme switch
        {
            Theme.System => Adw.ColorScheme.PreferLight,
            Theme.Light => Adw.ColorScheme.ForceLight,
            Theme.Dark => Adw.ColorScheme.ForceDark,
            _ => Adw.ColorScheme.PreferLight
        };
        //Main Window
        var mainWindow = new MainWindow(mainWindowController);
        _application.AddWindow(mainWindow);
        mainWindow.Show();
    }
}
