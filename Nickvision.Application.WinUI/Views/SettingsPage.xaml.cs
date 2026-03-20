using Microsoft.UI.Xaml;
using Microsoft.UI.Xaml.Controls;
using Nickvision.Application.Shared.Controllers;
using Nickvision.Desktop.Globalization;
using System.Collections.Generic;
using System.Threading.Tasks;

namespace Nickvision.Application.WinUI.Views;

public sealed partial class SettingsPage : Page
{
    private readonly PreferencesViewController _controller;
    private readonly ITranslationService _translationService;
    private bool _constructing;

    public SettingsPage(PreferencesViewController controller, ITranslationService translationService)
    {
        InitializeComponent();
        _controller = controller;
        _translationService = translationService;
        _constructing = true;
        // Translations
        LblSettings.Text = _translationService._("Settings");
        SelectorUI.Text = _translationService._("User Interface");
        RowTheme.Header = _translationService._("Theme");
        var themeLabels = new List<string>();
        foreach (var theme in _controller.Themes)
        {
            themeLabels.Add(theme.Label);
        }
        CmbTheme.ItemsSource = themeLabels;
        RowTranslationLanguage.Header = _translationService._("Translation Language");
        RowTranslationLanguage.Description = _translationService._("An application restart is required for a change to take effect");
        var languageLabels = new List<string>();
        foreach (var lang in _controller.AvailableTranslationLanguages)
        {
            languageLabels.Add(lang.Label);
        }
        CmbTranslationLanguage.ItemsSource = languageLabels;
        RowPreviewUpdates.Header = _translationService._("Receive Preview Updates");
        TglPreviewUpdates.OnContent = _translationService._("On");
        TglPreviewUpdates.OffContent = _translationService._("Off");
    }

    private void Page_Loaded(object sender, RoutedEventArgs e)
    {
        for (int i = 0; i < _controller.Themes.Count; i++)
        {
            if (_controller.Themes[i].ShouldSelect)
            {
                CmbTheme.SelectedIndex = i;
                break;
            }
        }
        for (int i = 0; i < _controller.AvailableTranslationLanguages.Count; i++)
        {
            if (_controller.AvailableTranslationLanguages[i].ShouldSelect)
            {
                CmbTranslationLanguage.SelectedIndex = i;
                break;
            }
        }
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
        if (CmbTheme.SelectedIndex < 0 || CmbTranslationLanguage.SelectedIndex < 0)
        {
            return;
        }
        _controller.Theme = _controller.Themes[CmbTheme.SelectedIndex];
        _controller.TranslationLanguage = _controller.AvailableTranslationLanguages[CmbTranslationLanguage.SelectedIndex];
        _controller.AllowPreviewUpdates = TglPreviewUpdates.IsOn;
        await _controller.SaveConfigurationAsync();
    }
}
