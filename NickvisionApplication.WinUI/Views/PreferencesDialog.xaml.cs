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
    }

    private void Dialog_Opened(ContentDialog sender, ContentDialogOpenedEventArgs args)
    {
        CmbTheme.SelectedIndex = (int)_controller.Theme;
    }

    private void Dialog_Closed(ContentDialog sender, ContentDialogClosedEventArgs args)
    {
        _controller.Theme = (Theme)CmbTheme.SelectedIndex;
        _controller.Save();
    }
}
