using Microsoft.UI.Xaml;
using Nickvision.Application.Shared.Controllers;
using Nickvision.Application.WinUI.Views;
using System;

namespace Nickvision.Application.WinUI;

public partial class App : Microsoft.UI.Xaml.Application
{
    private Window? _window;

    public App()
    {
        InitializeComponent();
    }

    protected override void OnLaunched(LaunchActivatedEventArgs args)
    {
        if (_window is null)
        {
            _window = new MainWindow(new MainWindowController(Environment.GetCommandLineArgs()));
        }
        _window.Activate();
    }
}
