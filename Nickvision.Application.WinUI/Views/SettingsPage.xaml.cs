using Microsoft.UI.Xaml;
using Microsoft.UI.Xaml.Controls;
using Nickvision.Application.Shared.Controllers;
using Nickvision.Application.Shared.Models;
using Nickvision.Desktop.Globalization;
using Nickvision.Desktop.WinUI.Helpers;
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
        NavigationView.PaneTitle = _translationService._("Settings");
        NavUserInterface.Content = _translationService._("User Interface");
        RowTheme.Header = _translationService._("Theme");
        CmbTheme.ItemsSource = _controller.Themes.ToBindableSelectonItems();
        RowTranslationLanguage.Header = _translationService._("Translation Language");
        RowTranslationLanguage.Description = _translationService._("An application restart is required for a change to take effect");
        CmbTranslationLanguage.ItemsSource = _controller.AvailableTranslationLanguages.ToBindableSelectonItems();
        RowPreviewUpdates.Header = _translationService._("Receive Preview Updates");
        TglPreviewUpdates.OnContent = _translationService._("On");
        TglPreviewUpdates.OffContent = _translationService._("Off");
    }

    private void Page_Loaded(object sender, RoutedEventArgs args)
    {
        CmbTheme.SelectSelectionItem();
        CmbTranslationLanguage.SelectSelectionItem();
        TglPreviewUpdates.IsOn = _controller.AllowPreviewUpdates;
        _constructing = false;
    }

    private async void Page_Unloaded(object sender, RoutedEventArgs args) => await _controller.SaveConfigurationAsync();

    private void NavigationView_SelectionChanged(NavigationView sender, NavigationViewSelectionChangedEventArgs args) => ViewStack.SelectedIndex = int.Parse(((NavigationView.SelectedItem as NavigationViewItem)!.Tag as string)!);

    private async void Cmb_SelectionChanged(object sender, SelectionChangedEventArgs args)
    {
        await ApplyChangesAsync();
        if(sender.Equals(CmbTheme))
        {
            await _controller.SaveConfigurationAsync();
        }
    }

    private async void Tgl_Toggled(object sender, RoutedEventArgs args) => await ApplyChangesAsync();

    private async Task ApplyChangesAsync()
    {
        if (_constructing)
        {
            return;
        }
        _controller.Theme = (CmbTheme.SelectedItem as BindableSelectionItem)!.ToSelectionItem<Theme>()!;
        _controller.TranslationLanguage = (CmbTranslationLanguage.SelectedItem as BindableSelectionItem)!.ToSelectionItem<string>()!;
        _controller.AllowPreviewUpdates = TglPreviewUpdates.IsOn;
    }
}
