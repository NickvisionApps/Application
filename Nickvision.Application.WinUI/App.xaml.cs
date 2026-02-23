using Microsoft.Extensions.DependencyInjection;
using Microsoft.UI.Xaml;
using Nickvision.Application.WinUI.Views;
using System;

namespace Nickvision.Application.WinUI;

public partial class App : Microsoft.UI.Xaml.Application
{
    private readonly IServiceProvider _serviceProvider;
    private Window? _window;

    public App(IServiceProvider serviceProvider)
    {
        InitializeComponent();
        _serviceProvider = serviceProvider;
    }

    protected override void OnLaunched(LaunchActivatedEventArgs args)
    {
        if (_window is null)
        {
            _window = ActivatorUtilities.CreateInstance<MainWindow>(_serviceProvider);
        }
        _window.Activate();
    }
}
