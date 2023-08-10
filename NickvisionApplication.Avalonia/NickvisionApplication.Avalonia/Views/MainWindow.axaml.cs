using Avalonia.Controls;
using Avalonia.Controls.ApplicationLifetimes;
using Avalonia.Interactivity;
using NickvisionApplication.Shared.Controllers;
using System;

namespace NickvisionApplication.Avalonia.Views;

public partial class MainWindow : Window
{
    private readonly MainWindowController _controller;
    private readonly IApplicationLifetime _lifetime;
    private bool _opened;
    
    public MainWindow(MainWindowController controller, IApplicationLifetime lifetime)
    {
        InitializeComponent();
        _controller = controller;
        _lifetime = lifetime;
        _opened = false;
        //Window Settings
        Title = $"{_controller.AppInfo.ShortName}{(_controller.AppInfo.IsDevVersion ? " - DEV" : "")}";
        Width = 800;
        Height = 600;
    }

    private void MainContent_OnLoaded(object? sender, RoutedEventArgs e)
    {
        if (!_opened)
        {
            _controller.Startup(_lifetime is IClassicDesktopStyleApplicationLifetime desktop ? (desktop.Args ?? Array.Empty<string>()) : Array.Empty<string>());
            _opened = true;
        }
    }
}