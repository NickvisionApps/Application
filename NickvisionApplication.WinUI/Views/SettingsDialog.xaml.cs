using Microsoft.UI.Xaml.Controls;
using Microsoft.Windows.AppLifecycle;
using NickvisionApplication.Shared.Controllers;
using NickvisionApplication.Shared.Models;
using System;
using System.Threading.Tasks;

namespace NickvisionApplication.WinUI.Views;

public sealed partial class SettingsDialog : ContentDialog
{
    private readonly PreferencesViewController _controller;

    public SettingsDialog(PreferencesViewController controller)
    {
        InitializeComponent();
        _controller = controller;
        //Localize Strings
        Title = _controller.Localizer["Settings"];
        PrimaryButtonText = _controller.Localizer["Apply"];
        CloseButtonText = _controller.Localizer["Cancel"];
        LblUserInterface.Text = _controller.Localizer["UserInterface"];
        LblUserInterfaceDescription.Text = _controller.Localizer["UserInterfaceDescription"];
        CardTheme.Header = _controller.Localizer["Theme"];
        CardTheme.Description = _controller.Localizer["ThemeDescription", "WinUI"];
        CmbTheme.Items.Add(_controller.Localizer["ThemeLight"]);
        CmbTheme.Items.Add(_controller.Localizer["ThemeDark"]);
        CmbTheme.Items.Add(_controller.Localizer["ThemeSystem"]);
        //Load Config
        CmbTheme.SelectedIndex = (int)_controller.Theme;
    }

    public new async Task<ContentDialogResult> ShowAsync()
    {
        var result = await base.ShowAsync();
        if(result == ContentDialogResult.Primary)
        {
            var needsRestart = false;
            if (_controller.Theme != (Theme)CmbTheme.SelectedIndex)
            {
                _controller.Theme = (Theme)CmbTheme.SelectedIndex;
                needsRestart = true;
            }
            _controller.SaveConfiguration();
            if(needsRestart)
            {
                var restartDialog = new ContentDialog()
                {
                    Title = _controller.Localizer["RestartThemeTitle", "WinUI"],
                    Content = string.Format(_controller.Localizer["RestartThemeDescription", "WinUI"], _controller.AppInfo.ShortName),
                    PrimaryButtonText = _controller.Localizer["Yes"],
                    CloseButtonText = _controller.Localizer["No"],
                    DefaultButton = ContentDialogButton.Close,
                    XamlRoot = XamlRoot
                };
                var resultRestart = await restartDialog.ShowAsync();
                if (resultRestart == ContentDialogResult.Primary)
                {
                    AppInstance.Restart("Apply new theme");
                }
            }
        }
        return result;
    }
}
