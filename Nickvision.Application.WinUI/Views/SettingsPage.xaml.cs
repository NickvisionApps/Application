using Microsoft.UI.Xaml;
using Microsoft.UI.Xaml.Controls;
using Nickvision.Application.Shared.Controllers;
using Nickvision.Application.Shared.Models;
using Nickvision.Desktop.Application;
using Nickvision.Desktop.WinUI.Helpers;
using System.Threading.Tasks;

namespace Nickvision.Application.WinUI.Views;

public sealed partial class SettingsPage : Page
{
    private PreferencesViewController _controller;
    private bool _constructing;

    public SettingsPage(PreferencesViewController controller)
    {
        InitializeComponent();
        _controller = controller;
        _constructing = true;
        // Translations
        LblSettings.Text = _controller.Translator._("Settings");
        SelectorUI.Text = _controller.Translator._("User Interface");
        RowTheme.Header = _controller.Translator._("Theme");
        CmbTheme.ItemsSource = _controller.Themes;
        RowTranslationLanguage.Header = _controller.Translator._("Translation Language");
        RowTranslationLanguage.Description = _controller.Translator._("An application restart is required for a change to take effect");
        CmbTranslationLanguage.ItemsSource = _controller.AvailableTranslationLanguages;
        RowPreviewUpdates.Header = _controller.Translator._("Receive Preview Updates");
        TglPreviewUpdates.OnContent = _controller.Translator._("On");
        TglPreviewUpdates.OffContent = _controller.Translator._("Off");
    }

    private void Page_Loaded(object sender, RoutedEventArgs e)
    {
        CmbTheme.SelectSelectionItem();
        CmbTranslationLanguage.SelectSelectionItem();
        TglPreviewUpdates.IsOn = _controller.AllowPreviewUpdates;
        _constructing = false;
    }

    private void SelectorBar_SelectionChanged(SelectorBar sender, SelectorBarSelectionChangedEventArgs args)
    {
        var index = sender.Items.IndexOf(sender.SelectedItem);
        ViewStack.SelectedIndex = index == -1 ? 0 : index;
    }

    private async void Cmb_SelectionChanged(object sender, SelectionChangedEventArgs e) => await ApplyChangesAsync();

    private async void Tgl_Toggled(object sender, Microsoft.UI.Xaml.RoutedEventArgs e) => await ApplyChangesAsync();

    private async Task ApplyChangesAsync()
    {
        if (_constructing)
        {
            return;
        }
        _controller.Theme = (CmbTheme.SelectedItem as SelectionItem<Theme>)!;
        _controller.TranslationLanguage = (CmbTranslationLanguage.SelectedItem as SelectionItem<string>)!;
        _controller.AllowPreviewUpdates = TglPreviewUpdates.IsOn;
        await _controller.SaveConfigurationAsync();
    }
}
