using Avalonia.Controls;
using Avalonia.Interactivity;
using NickvisionApplication.Shared.Controllers;

namespace NickvisionApplication.WinUI.Views;

/// <summary>
/// The MainWindow for the application
/// </summary>
public partial class MainWindow : Window
{
    private readonly MainWindowController _controller;

    /// <summary>
    /// Constructs a MainWindow
    /// </summary>
    public MainWindow() 
    {
        InitializeComponent();
        _controller = new MainWindowController();
        Title = "WinUI";
    }

    /// <summary>
    /// Constructs a MainWindow
    /// </summary>
    /// <param name="controller">MainWindowController</param>
    public MainWindow(MainWindowController controller)
    {
        InitializeComponent();
        _controller = controller;
        Title = _controller.AppInfo.ShortName;
        //Localize Strings
        MenuFile.Header = _controller.Localizer["File"];
        MenuOpenFolder.Header = _controller.Localizer["OpenFolder"];
        MenuCloseFolder.Header = _controller.Localizer["CloseFolder"];
        MenuExit.Header = _controller.Localizer["Exit"];
        MenuEdit.Header = _controller.Localizer["Edit"];
        MenuSettings.Header = _controller.Localizer["Settings"];
        MenuHelp.Header = _controller.Localizer["Help"];
        MenuAbout.Header = string.Format(_controller.Localizer["About"], _controller.AppInfo.ShortName);
        LblStatus.Text = _controller.Localizer["StatusReady", "WinUI"];
    }

    /// <summary>
    /// Occurs when the exit menu item is clicked
    /// </summary>
    /// <param name="sender">object?</param>
    /// <param name="e">RoutedEventArgs</param>
    private void Exit(object? sender, RoutedEventArgs e) => Close();
}