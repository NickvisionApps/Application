using Avalonia;
using Avalonia.Controls.ApplicationLifetimes;
using Avalonia.Markup.Xaml;
using NickvisionApplication.Mobile.Views;
using NickvisionApplication.Shared.Controllers;

namespace NickvisionApplication.Mobile;

public partial class App : Application
{
    private MainWindowController _mainWindowController;
    
    public override void Initialize()
    {
        AvaloniaXamlLoader.Load(this);
        _mainWindowController = new MainWindowController();
        _mainWindowController.AppInfo.Changelog = "* Initial Release";
    }

    public override void OnFrameworkInitializationCompleted()
    {
        if (ApplicationLifetime is IClassicDesktopStyleApplicationLifetime desktop)
        {
            desktop.MainWindow = new MainWindow(_mainWindowController, desktop);
        }
        else if (ApplicationLifetime is ISingleViewApplicationLifetime singleViewPlatform)
        {
            singleViewPlatform.MainView = new MainView(_mainWindowController, singleViewPlatform);
        }
        base.OnFrameworkInitializationCompleted();
    }
}