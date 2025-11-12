using Microsoft.UI.Windowing;
using Microsoft.UI.Xaml;
using Microsoft.UI.Xaml.Controls;
using Nickvision.Application.Shared.Controllers;
using Nickvision.Application.Shared.Models;

namespace Nickvision.Application.WinUI.Views;

public sealed partial class MainWindow : Window
{
    private readonly MainWindowController _controller;

    public MainWindow(MainWindowController controller)
    {
        InitializeComponent();
        _controller = controller;
        // Theme
        MainGrid.RequestedTheme = _controller.Theme switch
        {
            Theme.Light => ElementTheme.Light,
            Theme.Dark => ElementTheme.Dark,
            _ => ElementTheme.Default
        };
        // TitleBar
        AppWindow.SetIcon("./Assets/org.nickvision.application.ico");
        ExtendsContentIntoTitleBar = true;
        SetTitleBar(TitleBar);
        AppWindow.TitleBar.PreferredHeightOption = TitleBarHeightOption.Tall;
        // Events
        // Translations
        AppWindow.Title = _controller.AppInfo.ShortName;
        TitleBar.Title = _controller.AppInfo.ShortName;
        TitleBar.Subtitle = _controller.AppInfo.Version!.IsPreview ? _controller.Translator._("Preview") : string.Empty;
        MenuFile.Title = _controller.Translator._("File");
        MenuOpenFolder.Text = _controller.Translator._("Open Folder");
        MenuCloseFolder.Text = _controller.Translator._("Close Folder");
        MenuExit.Text = _controller.Translator._("Exit");
        MenuEdit.Title = _controller.Translator._("Edit");
        MenuSettings.Text = _controller.Translator._("Settings");
        MenuHelp.Title = _controller.Translator._("Help");
        ToolTipService.SetToolTip(BtnSettings, _controller.Translator._("Settings"));
    }
}
