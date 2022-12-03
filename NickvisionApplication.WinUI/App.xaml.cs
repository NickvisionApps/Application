using Microsoft.UI.Xaml;
using NickvisionApplication.Shared.Models;
using NickvisionApplication.WinUI.Views;
using System;

namespace NickvisionApplication.WinUI;

public partial class App : Application
{
    private Window? _mainWindow;

    public App()
    {
        InitializeComponent();
        //Set AppInfo
        AppInfo.Current.ID = "org.nickvision.application";
        AppInfo.Current.Name = "NickvisionApplication";
        AppInfo.Current.ShortName = "Application";
        AppInfo.Current.Description = "A template for creating Nickvision applications.";
        AppInfo.Current.Version = "2022.12.0-next";
        AppInfo.Current.Changelog = "- Initial Release";
        AppInfo.Current.GitHubRepo = new Uri("https://github.com/nlogozzo/NickvisionApplication");
        AppInfo.Current.IssueTracker = new Uri("https://github.com/nlogozzo/NickvisionApplication/issues/new");
        AppInfo.Current.SupportUrl = new Uri("https://github.com/nlogozzo/NickvisionApplication/discussions");
    }

    protected override void OnLaunched(LaunchActivatedEventArgs args)
    {
        _mainWindow = new MainWindow();
        _mainWindow.Activate();
    }
}
