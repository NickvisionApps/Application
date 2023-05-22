using Avalonia;
using Avalonia.Controls;
using Avalonia.Styling;
using FluentAvalonia.UI.Controls;
using NickvisionApplication.Shared.Controllers;
using NickvisionApplication.Shared.Models;
using System;
using System.Collections.Generic;
using System.Threading.Tasks;

namespace NickvisionApplication.Fluent.Views;

public partial class SettingsDialog : ContentDialog, IStyleable
{
    Type IStyleable.StyleKey => typeof(ContentDialog);

    private readonly PreferencesViewController _controller;
    
    public SettingsDialog() => InitializeComponent();

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
        LblThemeHeader.Text = _controller.Localizer["Theme"];
        LblThemeDescription.Text = _controller.Localizer["ThemeDescription", "WinUI"];
        CmbTheme.Items = new List<string>() { _controller.Localizer["ThemeLight"], _controller.Localizer["ThemeDark"], _controller.Localizer["ThemeSystem"] };
        //Load Config
        CmbTheme.SelectedIndex = (int)_controller.Theme;
    }

    public new async Task<ContentDialogResult> ShowAsync(Window window, ContentDialogPlacement placement = ContentDialogPlacement.Popup)
    {
        var result = await base.ShowAsync(window, placement);
        if (result == ContentDialogResult.Primary)
        {
            var needsRestart = false;
            if (_controller.Theme != (Theme)CmbTheme.SelectedIndex)
            {
                _controller.Theme = (Theme)CmbTheme.SelectedIndex;
                needsRestart = true;
            }
            _controller.SaveConfiguration();
            if (needsRestart)
            {
                var restartDialog = new ContentDialog()
                {
                    Title = _controller.Localizer["RestartThemeTitle", "WinUI"],
                    Content = string.Format(_controller.Localizer["RestartThemeDescription", "WinUI"], _controller.AppInfo.ShortName),
                    CloseButtonText = _controller.Localizer["OK"],
                    DefaultButton = ContentDialogButton.Close
                };
                await restartDialog.ShowAsync();
            }
        }
        return result;
    }
    
    /// <summary>
    /// Occurs when the ScrollViewer's scrolling is changed
    /// </summary>
    /// <param name="sender"></param>
    /// <param name="e"></param>
    private void ScrollViewer_ScrollChanged(object? sender, ScrollChangedEventArgs e) => StackPanel.Margin = new Thickness(0, 0, ScrollViewer.Extent.Height > ScrollViewer.Viewport.Height ? 14 : 0, 0);
}