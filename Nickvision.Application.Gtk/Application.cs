using Nickvision.Application.Gtk.Views;
using Nickvision.Application.Shared.Controllers;
using Nickvision.Application.Shared.Models;
using System;

namespace Nickvision.Application.Gtk;

public class Application
{
    private string[] _args;
    private MainWindowController _controller;
    private Adw.Application _application;
    private MainWindow? _mainWindow;

    public Application(string[] args)
    {
        _args = args;
        _controller = new MainWindowController(_args);
        _application = Adw.Application.New(_controller.AppInfo.Id, Gio.ApplicationFlags.DefaultFlags);
        _application.OnStartup += Application_OnStartup;
        _application.OnActivate += Application_OnActivate;
    }

    public int Run() => _application.RunWithSynchronizationContext(_args);

    private void Application_OnStartup(Gio.Application sender, EventArgs args)
    {
        if(_mainWindow is null)
        {
            _mainWindow = new MainWindow(_controller)
            {
                Application = _application
            };
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
