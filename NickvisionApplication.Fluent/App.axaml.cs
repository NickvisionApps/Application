using Avalonia;
using Avalonia.Controls.ApplicationLifetimes;
using Avalonia.Markup.Xaml;
using NickvisionApplication.Shared.Controllers;
using NickvisionApplication.Fluent.Views;
using System;

namespace NickvisionApplication.Fluent;

/// <summary>
/// The App
/// </summary>
public partial class App : Application
{
    private MainWindowController _mainWindowController;

    /// <summary>
    /// Occurs when Avalonia is initialized
    /// </summary>
    public override void Initialize()
    {
        AvaloniaXamlLoader.Load(this);
        _mainWindowController = new MainWindowController();
        //AppInfo
        _mainWindowController.AppInfo.ID = "org.nickvision.application";
        _mainWindowController.AppInfo.Name = "NickvisionApplication";
        _mainWindowController.AppInfo.ShortName = _mainWindowController.Localizer["ShortName"];
        _mainWindowController.AppInfo.Description = $"{_mainWindowController.Localizer["Description"]}.";
        _mainWindowController.AppInfo.Version = "2023.5.0-next";
        _mainWindowController.AppInfo.Changelog = "- Initial Release";
        _mainWindowController.AppInfo.GitHubRepo = new Uri("https://github.com/NickvisionApps/Application");
        _mainWindowController.AppInfo.IssueTracker = new Uri("https://github.com/NickvisionApps/Application/issues/new");
        _mainWindowController.AppInfo.SupportUrl = new Uri("https://github.com/NickvisionApps/Application/discussions");
    }

    /// <summary>
    /// Occurs when Avalonia has finished initialization
    /// </summary>
    public override void OnFrameworkInitializationCompleted()
    {
        if (ApplicationLifetime is IClassicDesktopStyleApplicationLifetime desktop)
        {
            desktop.MainWindow = new MainWindow(_mainWindowController);
        }
        base.OnFrameworkInitializationCompleted();
    }
}