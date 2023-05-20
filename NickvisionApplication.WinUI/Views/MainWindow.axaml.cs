using Avalonia.Controls;
using NickvisionApplication.Shared.Controllers;

namespace NickvisionApplication.WinUI.Views;

public partial class MainWindow : Window
{
    private readonly MainWindowController _controller;

    public MainWindow() 
    {
        InitializeComponent();
        _controller = new MainWindowController();
        Title = "WinUI";
    }

    public MainWindow(MainWindowController controller)
    {
        InitializeComponent();
        _controller = controller;
        Title = _controller.AppInfo.ShortName;
    }
}