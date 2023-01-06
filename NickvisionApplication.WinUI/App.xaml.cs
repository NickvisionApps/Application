using Microsoft.UI.Xaml;
using NickvisionApplication.Shared.Controllers;
using NickvisionApplication.Shared.Models;
using NickvisionApplication.WinUI.Views;
using System;

namespace NickvisionApplication.WinUI;

public partial class App : Application
{
    private Window? _mainWindow;
    private MainWindowController _mainWindowController;

    public App()
    {
        InitializeComponent();
        _mainWindowController = new MainWindowController();
        //AppInfo
        _mainWindowController.AppInfo.ID = "org.nickvision.application";
        _mainWindowController.AppInfo.Name = "NickvisionApplication";
        _mainWindowController.AppInfo.ShortName = "Application";
        _mainWindowController.AppInfo.Description = $"{_mainWindowController.Localizer["Description"]}.";
        _mainWindowController.AppInfo.Version = "2022.12.0-next";
        _mainWindowController.AppInfo.Changelog = "- Initial Release";
        _mainWindowController.AppInfo.GitHubRepo = new Uri("https://github.com/nlogozzo/NickvisionApplication");
        _mainWindowController.AppInfo.IssueTracker = new Uri("https://github.com/nlogozzo/NickvisionApplication/issues/new");
        _mainWindowController.AppInfo.SupportUrl = new Uri("https://github.com/nlogozzo/NickvisionApplication/discussions");
        //Theme
        if (_mainWindowController.Theme == Theme.Light)
        {
            RequestedTheme = ApplicationTheme.Light;
        }
        else if(_mainWindowController.Theme == Theme.Dark)
        {
            RequestedTheme = ApplicationTheme.Dark;
        }
    }

    protected override void OnLaunched(LaunchActivatedEventArgs args)
    {
        _mainWindow = new MainWindow(_mainWindowController);
        _mainWindow.Activate();
    }
}
