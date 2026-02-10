using Nickvision.Application.GNOME.Views;
using Nickvision.Application.Shared.Controllers;
using Nickvision.Application.Shared.Models;
using System;
using System.IO;

namespace Nickvision.Application.GNOME;

public class Application
{
    private readonly string[] _args;
    private readonly MainWindowController _controller;
    private readonly Adw.Application _application;
    private readonly Gio.Resource _resource;
    private MainWindow? _mainWindow;

    public Application(string[] args)
    {
        _args = new string[args.Length + 1];
        _args[0] = "org.nickvision.application";
        args.CopyTo(_args, 1);
        _controller = new MainWindowController(_args);
        _application = Adw.Application.New(_controller.AppInfo.Id, Gio.ApplicationFlags.DefaultFlags);
        var resourceFilePath = Path.Combine(Desktop.System.Environment.ExecutingDirectory, $"{_controller.AppInfo.Id}.gresource");
        try
        {
            _resource = Gio.Resource.Load(resourceFilePath);
            _resource.Register();
        }
        catch (Exception ex)
        {
            throw new Exception($"Failed to load GResource file: {resourceFilePath}", ex);
        }
        _application.OnStartup += Application_OnStartup;
        _application.OnActivate += Application_OnActivate;
    }

    public int Run() => _application.RunWithSynchronizationContext(_args);

    private void Application_OnStartup(Gio.Application sender, EventArgs args)
    {
        if (_mainWindow is null)
        {
            _mainWindow = new MainWindow(_controller, _application);
            _application.AddWindow(_mainWindow);
        }
        Adw.StyleManager.GetDefault().ColorScheme = _controller.Theme switch
        {
            Theme.Light => Adw.ColorScheme.ForceLight,
            Theme.Dark => Adw.ColorScheme.ForceDark,
            _ => Adw.ColorScheme.Default
        };
    }

    private void Application_OnActivate(Gio.Application sender, EventArgs args) => _mainWindow?.Present();
}
