using NickvisionApplication.GNOME.Views;
using NickvisionApplication.Shared.Controllers;
using NickvisionApplication.Shared.Models;
using System;
using System.Collections.Generic;
using System.IO;
using System.Reflection;
using System.Runtime.InteropServices;

namespace NickvisionApplication.GNOME;

/// <summary>
/// The Program 
/// </summary>
public partial class Program
{
    [LibraryImport("libadwaita-1.so.0", StringMarshalling = StringMarshalling.Utf8)]
    private static partial nuint gtk_file_chooser_cell_get_type();

    [LibraryImport("libadwaita-1.so.0", StringMarshalling = StringMarshalling.Utf8)]
    private static partial nint g_resource_load(string path);

    [LibraryImport("libadwaita-1.so.0", StringMarshalling = StringMarshalling.Utf8)]
    private static partial void g_resources_register(nint file);

    private readonly Adw.Application _application;
    private MainWindow? _mainWindow;
    private MainWindowController _mainWindowController;

    /// <summary>
    /// Main method
    /// </summary>
    /// <param name="args">string[]</param>
    /// <returns>Return code from Adw.Application.Run()</returns>
    public static int Main(string[] args) => new Program().Run(args);

    /// <summary>
    /// Constructs a Program
    /// </summary>
    public Program()
    {
        gtk_file_chooser_cell_get_type();
        _application = Adw.Application.New("org.nickvision.application", Gio.ApplicationFlags.FlagsNone);
        _mainWindow = null;
        _mainWindowController = new MainWindowController();
        _mainWindowController.AppInfo.ID = "org.nickvision.application";
        _mainWindowController.AppInfo.Name = "NickvisionApplication";
        _mainWindowController.AppInfo.ShortName = _mainWindowController.Localizer["ShortName"];
        _mainWindowController.AppInfo.Description = $"{_mainWindowController.Localizer["Description"]}.";
        _mainWindowController.AppInfo.Version = "2023.4.0-next";
        _mainWindowController.AppInfo.Changelog = "<ul><li>Initial Release</li></ul>";
        _mainWindowController.AppInfo.GitHubRepo = new Uri("https://github.com/NickvisionApps/Application");
        _mainWindowController.AppInfo.IssueTracker = new Uri("https://github.com/NickvisionApps/Application/issues/new");
        _mainWindowController.AppInfo.SupportUrl = new Uri("https://github.com/NickvisionApps/Application/discussions");
        _application.OnActivate += OnActivate;
        if (File.Exists(Path.GetFullPath(System.IO.Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location)) + "/org.nickvision.application.gresource"))
        {
            //Load file from program directory, required for `dotnet run`
            g_resources_register(g_resource_load(Path.GetFullPath(System.IO.Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location)) + "/org.nickvision.application.gresource"));
        }
        else
        {
            var prefixes = new List<string> {
               Directory.GetParent(Directory.GetParent(Path.GetFullPath(System.IO.Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location))).FullName).FullName,
               Directory.GetParent(Path.GetFullPath(System.IO.Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location))).FullName,
               "/usr"
            };
            foreach (var prefix in prefixes)
            {
                if (File.Exists(prefix + "/share/org.nickvision.application/org.nickvision.application.gresource"))
                {
                    g_resources_register(g_resource_load(Path.GetFullPath(prefix + "/share/org.nickvision.application/org.nickvision.application.gresource")));
                    break;
                }
            }
        }
    }

    /// <summary>
    /// Runs the program
    /// </summary>
    /// <returns>Return code from Adw.Application.Run()</returns>
    public int Run(string[] args)
    {
        try
        {
            var argv = new string[args.Length + 1];
            argv[0] = $"{_mainWindowController.AppInfo.Name}.GNOME";
            args.CopyTo(argv, 1);
            return _application.Run(args.Length + 1, argv);
        }
        catch (Exception ex)
        {
            Console.WriteLine(ex.Message);
            Console.WriteLine($"\n\n{ex.StackTrace}");
            return -1;
        }
    }

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
