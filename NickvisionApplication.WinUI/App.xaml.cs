using Microsoft.UI.Xaml;
using NickvisionApplication.Shared.Controllers;
using NickvisionApplication.Shared.Models;
using NickvisionApplication.WinUI.Views;
using System;

namespace NickvisionApplication.WinUI;


/// <summary>
/// The App
/// </summary>
public partial class App : Application
{
    public static Window? MainWindow { get; private set; } = null;
    private readonly MainWindowController _mainWindowController;

    /// <summary>
    /// Constructs an App
    /// </summary>
    public App()
    {
        InitializeComponent();
        _mainWindowController = new MainWindowController();
        //AppInfo
        _mainWindowController.AppInfo.ID = "org.nickvision.application";
        _mainWindowController.AppInfo.Name = "NickvisionApplication";
        _mainWindowController.AppInfo.ShortName = "Application";
        _mainWindowController.AppInfo.Description = $"{_mainWindowController.Localizer["Description"]}.";
        _mainWindowController.AppInfo.Version = "2023.4.0-next";
        _mainWindowController.AppInfo.Changelog = "- Initial Release";
        _mainWindowController.AppInfo.GitHubRepo = new Uri("https://github.com/nlogozzo/NickvisionApplication");
        _mainWindowController.AppInfo.IssueTracker = new Uri("https://github.com/nlogozzo/NickvisionApplication/issues/new");
        _mainWindowController.AppInfo.SupportUrl = new Uri("https://github.com/nlogozzo/NickvisionApplication/discussions");
        //Theme
        if (_mainWindowController.Theme == Theme.Light)
        {
            RequestedTheme = ApplicationTheme.Light;
        }
        else if (_mainWindowController.Theme == Theme.Dark)
        {
            RequestedTheme = ApplicationTheme.Dark;
        }
    }

    /// <summary>
    /// Occurs when the app is launched
    /// </summary>
    /// <param name="args">LaunchActivatedEventArgs</param>
    protected override void OnLaunched(LaunchActivatedEventArgs args)
    {
        MainWindow = new MainWindow(_mainWindowController);
        MainWindow.Activate();
    }
}
