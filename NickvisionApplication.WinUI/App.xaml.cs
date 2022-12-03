using Microsoft.UI.Xaml;
using NickvisionApplication.Shared.Controllers;
using NickvisionApplication.WinUI.Views;
using System;

namespace NickvisionApplication.WinUI;

public partial class App : Application
{
    private Window? _mainWindow;

    public App()
    {
        InitializeComponent();
    }

    protected override void OnLaunched(LaunchActivatedEventArgs args)
    {
        var controller = new MainWindowController();
        controller.AppInfo.ID = "org.nickvision.application";
        controller.AppInfo.Name = "NickvisionApplication";
        controller.AppInfo.ShortName = "Application";
        controller.AppInfo.Description = "A template for creating Nickvision applications.";
        controller.AppInfo.Version = "2022.12.0-next";
        controller.AppInfo.Changelog = "- Initial Release";
        controller.AppInfo.GitHubRepo = new Uri("https://github.com/nlogozzo/NickvisionApplication");
        controller.AppInfo.IssueTracker = new Uri("https://github.com/nlogozzo/NickvisionApplication/issues/new");
        controller.AppInfo.SupportUrl = new Uri("https://github.com/nlogozzo/NickvisionApplication/discussions");
        _mainWindow = new MainWindow(controller);
        _mainWindow.Activate();
    }
}
