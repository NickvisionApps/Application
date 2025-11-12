using Microsoft.UI.Xaml;
using Nickvision.Application.Shared.Controllers;
using Nickvision.Application.WinUI.Views;

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
        if(_window is null)
        {
            _window = new MainWindow(new MainWindowController(args.Arguments.Split(' ')));
        }
        _window.Activate();
    }
}
