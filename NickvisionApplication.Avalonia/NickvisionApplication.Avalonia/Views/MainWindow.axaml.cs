using Avalonia.Controls;
using Avalonia.Controls.ApplicationLifetimes;
using NickvisionApplication.Shared.Controllers;

namespace NickvisionApplication.Avalonia.Views;

/// <summary>
/// The MainWindow for the application
/// </summary>
public partial class MainWindow : Window
{
    /// <summary>
    /// Constructs a MainWindow
    /// </summary>
    /// <param name="controller">MainWindowController</param>
    /// <param name="lifetime">IApplicationLifetime</param>
    public MainWindow(MainWindowController controller, IApplicationLifetime lifetime)
    {
        InitializeComponent();
        Title = $"{controller.AppInfo.ShortName}{(controller.AppInfo.IsDevVersion ? " - DEV" : "")}";
        Content = new MainView(controller, lifetime);
    }
}