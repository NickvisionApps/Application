using NickvisionApplication.GNOME.Views;
using NickvisionApplication.Shared.Controllers;
using NickvisionApplication.Shared.Models;
using System;
// Uncomment if the application uses GResource file
//using System.Collections.Generic;

namespace NickvisionApplication.GNOME;

/// <summary>
/// The Program 
/// </summary>
public class Program
{
    private readonly Adw.Application _application;
    private MainWindow? _mainWindow;
    private MainWindowController _mainWindowController;

    /// <summary>
    /// Main method
    /// </summary>
    /// <param name="args">string[]</param>
    /// <returns>Return code from Adw.Application.Run()</returns>
    public static int Main(string[] args) => new Program().Run();

    /// <summary>
    /// Constructs a Program
    /// </summary>
    public Program()
    {
        _application = Adw.Application.New("org.nickvision.application", Gio.ApplicationFlags.FlagsNone);
        _mainWindow = null;
        _mainWindowController = new MainWindowController();
        _mainWindowController.AppInfo.ID = "org.nickvision.application";
        _mainWindowController.AppInfo.Name = "NickvisionApplication";
        _mainWindowController.AppInfo.ShortName = "Application";
        _mainWindowController.AppInfo.Description = $"{_mainWindowController.Localizer["Description"]}.";
        _mainWindowController.AppInfo.Version = "2023.2.0-next";
        _mainWindowController.AppInfo.Changelog = "<ul><li>Initial Release</li></ul>";
        _mainWindowController.AppInfo.GitHubRepo = new Uri("https://github.com/nlogozzo/NickvisionApplication");
        _mainWindowController.AppInfo.IssueTracker = new Uri("https://github.com/nlogozzo/NickvisionApplication/issues/new");
        _mainWindowController.AppInfo.SupportUrl = new Uri("https://github.com/nlogozzo/NickvisionApplication/discussions");
        _application.OnActivate += OnActivate;
        // Uncomment following lines if the application uses GResource file
        //var prefixes = new List<string> {
        //    Directory.GetParent(Directory.GetParent(Path.GetFullPath(System.IO.Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location))).FullName).FullName,
        //    Directory.GetParent(Path.GetFullPath(System.IO.Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location))).FullName,
        //    "/usr"
        //};
        //foreach(var prefix in prefixes)
        //{
        //    if(File.Exists(prefix + "/share/org.nickvision.money/org.nickvision.money.gresource"))
        //    {
        //        g_resources_register(g_resource_load(Path.GetFullPath(prefix + "/share/org.nickvision.money/org.nickvision.money.gresource")));
        //        break;
        //    }
        //}
    }

    /// <summary>
    /// Finalizes a Program
    /// </summary>
    ~Program() => _mainWindowController.Dispose();

    /// <summary>
    /// Runs the program
    /// </summary>
    /// <returns>Return code from Adw.Application.Run()</returns>
    public int Run() => _application.Run();

    /// <summary>
    /// Occurs when the application is activated
    /// </summary>
    /// <param name="sedner">Gio.Application</param>
    /// <param name="e">EventArgs</param>
    private void OnActivate(Gio.Application sedner, EventArgs e)
    {
        //Set Adw Theme
        _application.StyleManager!.ColorScheme = _mainWindowController.Theme switch
        {
            Theme.System => Adw.ColorScheme.PreferLight,
            Theme.Light => Adw.ColorScheme.ForceLight,
            Theme.Dark => Adw.ColorScheme.ForceDark,
            _ => Adw.ColorScheme.PreferLight
        };
        //Main Window
        _mainWindow = new MainWindow(_mainWindowController, _application);
        _mainWindow.Start();
    }
}
