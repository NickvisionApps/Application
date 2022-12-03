using Microsoft.UI.Xaml;
using NickvisionApplication.Shared;
using NickvisionApplication.WinUI.Views;

namespace NickvisionApplication.WinUI;

public partial class App : Application
{
    private Window _mainWindow;

    public App()
    {
        InitializeComponent();
        //Set AppInfo
    }

    protected override void OnLaunched(LaunchActivatedEventArgs args)
    {
        _mainWindow = new MainWindow();
        _mainWindow.Activate();
    }
}
