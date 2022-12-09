using Microsoft.UI.Xaml.Controls;
using NickvisionApplication.Shared.Controllers;
using NickvisionApplication.Shared.Models;

namespace NickvisionApplication.WinUI.Views;

public sealed partial class PreferencesDialog : ContentDialog
{
    private readonly PreferencesViewController _controller;

    public PreferencesDialog(PreferencesViewController controller)
    {
        InitializeComponent();
        _controller = controller;
        //Localize Strings
        Title = _controller.Localizer["Settings"];
        CardTheme.Header = _controller.Localizer["SettingsTheme"];
        CardTheme.Description = _controller.Localizer["SettingsThemeDescription"];
        CmbTheme.Items.Add(_controller.Localizer["SettingsThemeLight"]);
        CmbTheme.Items.Add(_controller.Localizer["SettingsThemeDark"]);
        CmbTheme.Items.Add(_controller.Localizer["SettingsThemeSystem"]);
    }

    private void Dialog_Opened(ContentDialog sender, ContentDialogOpenedEventArgs args)
    {
        CmbTheme.SelectedIndex = (int)_controller.Theme;
    }

    private void Dialog_Closed(ContentDialog sender, ContentDialogClosedEventArgs args)
    {
        _controller.Theme = (Theme)CmbTheme.SelectedIndex;
        _controller.SaveConfiguration();
    }
}
