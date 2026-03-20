using Microsoft.UI.Xaml;
using Microsoft.UI.Xaml.Controls;
using Nickvision.Application.Shared.Controllers;
using Nickvision.Desktop.Globalization;
using Nickvision.Desktop.WinUI.Helpers;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace Nickvision.Application.WinUI.Views;

public sealed partial class SettingsPage : Page
{
    private readonly PreferencesViewController _controller;
    private readonly ITranslationService _translationService;
    private readonly List<BindableSelectionItem> _bindableThemes;
    private readonly List<BindableSelectionItem> _bindableTranslationLanguages;
    private bool _constructing;

    public SettingsPage(PreferencesViewController controller, ITranslationService translationService)
    {
        InitializeComponent();
        _controller = controller;
        _translationService = translationService;
        _constructing = true;
        _bindableThemes = _controller.Themes.Select(t => new BindableSelectionItem(t)).ToList();
        _bindableTranslationLanguages = _controller.AvailableTranslationLanguages.Select(t => new BindableSelectionItem(t)).ToList();
        // Translations
        LblSettings.Text = _translationService._("Settings");
        SelectorUI.Text = _translationService._("User Interface");
        RowTheme.Header = _translationService._("Theme");
        CmbTheme.ItemsSource = _bindableThemes;
        RowTranslationLanguage.Header = _translationService._("Translation Language");
        RowTranslationLanguage.Description = _translationService._("An application restart is required for a change to take effect");
        CmbTranslationLanguage.ItemsSource = _bindableTranslationLanguages;
        RowPreviewUpdates.Header = _translationService._("Receive Preview Updates");
        TglPreviewUpdates.OnContent = _translationService._("On");
        TglPreviewUpdates.OffContent = _translationService._("Off");
    }

    private void Page_Loaded(object sender, RoutedEventArgs e)
    {
        CmbTheme.SelectedItem = _bindableThemes.FirstOrDefault(x => x.ShouldSelect);
        CmbTranslationLanguage.SelectedItem = _bindableTranslationLanguages.FirstOrDefault(x => x.ShouldSelect);
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
        if (CmbTheme.SelectedIndex >= 0 && CmbTheme.SelectedIndex < _controller.Themes.Count)
        {
            _controller.Theme = _controller.Themes[CmbTheme.SelectedIndex];
        }
        if (CmbTranslationLanguage.SelectedIndex >= 0 && CmbTranslationLanguage.SelectedIndex < _controller.AvailableTranslationLanguages.Count)
        {
            _controller.TranslationLanguage = _controller.AvailableTranslationLanguages[CmbTranslationLanguage.SelectedIndex];
        }
        _controller.AllowPreviewUpdates = TglPreviewUpdates.IsOn;
        await _controller.SaveConfigurationAsync();
    }
}
