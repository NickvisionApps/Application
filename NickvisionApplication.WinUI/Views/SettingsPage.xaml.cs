using Microsoft.UI.Xaml;
using Microsoft.UI.Xaml.Controls;
using Microsoft.Windows.AppLifecycle;
using NickvisionApplication.Shared.Controllers;
using NickvisionApplication.Shared.Models;
using System;
using System.Globalization;
using System.Linq;
using System.Threading.Tasks;
using Windows.ApplicationModel.DataTransfer;
using static Nickvision.Aura.Localization.Gettext;

namespace NickvisionApplication.WinUI.Views;

/// <summary>
/// A page for managing application settings
/// </summary>
public sealed partial class SettingsPage : UserControl
{
    private readonly PreferencesViewController _controller;
    private bool _constructing;

    /// <summary>
    /// Constructs a SettingsPage
    /// </summary>
    /// <param name="controller">PreferencesViewController</param>
    public SettingsPage(PreferencesViewController controller)
    {
        InitializeComponent();
        _controller = controller;
        _constructing = true;
        //Localize Strings
        LblTitle.Text = _("Settings");
        LblUserInterface.Text = _("User Interface");
        CardTheme.Header = _("Theme");
        CardTheme.Description = _("An application restart is required to change the theme.");
        CmbTheme.Items.Add(_p("Theme", "Light"));
        CmbTheme.Items.Add(_p("Theme", "Dark"));
        CmbTheme.Items.Add(_p("Theme", "System"));
        CardAutomaticallyCheckForUpdates.Header = _("Automatically Check for Updates");
        TglAutomaticallyCheckForUpdates.OnContent = _("On");
        TglAutomaticallyCheckForUpdates.OffContent = _("Off");
        LblAbout.Text = _("About");
        BtnCopyDebugInformation.Content = _("Copy Debug Information");
        CardChangelog.Header = _("Changelog");
        CardCredits.Header = _("Credits");
        //Load Config
        CmbTheme.SelectedIndex = (int)_controller.Theme;
        TglAutomaticallyCheckForUpdates.IsOn = _controller.AutomaticallyCheckForUpdates;
        //Load About
        CardAbout.Header = _controller.AppInfo.ShortName;
        CardAbout.Description = _controller.AppInfo.Description;
        LblVersion.Text = _controller.AppInfo.Version;
        CardChangelog.Description = _controller.AppInfo.Changelog;
        CardCredits.Description = _("Developers:\n{0}\n\nDesigners:\n{1}\n\nArtists:\n{2}\n\nTranslators:\n{3}", string.Join("\n", _controller.AppInfo.Developers.Keys), string.Join("\n", _controller.AppInfo.Designers.Keys), string.Join("\n", _controller.AppInfo.Artists.Keys), string.Join("\n", _controller.AppInfo.TranslatorNames.Where(x => x != "translator-credits")));
        _constructing = false;
    }

    /// <summary>
    /// Applies the changes to the app's configuration
    /// </summary>
    private async Task ApplyChangesAsync()
    {
        if(_constructing)
        {
            return;
        }
        var needsRestart = false;
        if (_controller.Theme != (Theme)CmbTheme.SelectedIndex)
        {
            _controller.Theme = (Theme)CmbTheme.SelectedIndex;
            needsRestart = true;
        }
        _controller.AutomaticallyCheckForUpdates = TglAutomaticallyCheckForUpdates.IsOn;
        _controller.SaveConfiguration();
        if (needsRestart)
        {
            var restartDialog = new ContentDialog()
            {
                Title = _("Restart To Apply Theme?"),
                Content = _("Would you like to restart {0} to apply the new theme?\nAny unsaved work will be lost.", _controller.AppInfo.ShortName),
                PrimaryButtonText = _("Yes"),
                CloseButtonText = _("No"),
                DefaultButton = ContentDialogButton.Primary,
                XamlRoot = XamlRoot
            };
            var resultRestart = await restartDialog.ShowAsync();
            if (resultRestart == ContentDialogResult.Primary)
            {
                AppInstance.Restart("Apply new theme");
            }
        }
    }

    /// <summary>
    /// Occurs when the CmbTheme's selection is changed
    /// </summary>
    /// <param name="sender">object</param>
    /// <param name="e">SelectionChangedEventArgs</param>
    private async void CmbTheme_SelectionChanged(object sender, SelectionChangedEventArgs e) => await ApplyChangesAsync();

    /// <summary>
    /// Occurs when the TglAutomaticallyCheckForUpdates is toggled
    /// </summary>
    /// <param name="sender">object</param>
    /// <param name="e">RoutedEventArgs</param>
    private async void TglAutomaticallyCheckForUpdates_Toggled(object sender, RoutedEventArgs e) => await ApplyChangesAsync();

    /// <summary>
    /// Occurs when the BtnCopyDebugInformation is clicked
    /// </summary>
    /// <param name="sender">object</param>
    /// <param name="e">RoutedEventArgs</param>
    private void CopyDebugInformation(object sender, RoutedEventArgs e)
    {
        var dataPackage = new DataPackage();
        dataPackage.SetText($"{_controller.AppInfo.ID}\n{_controller.AppInfo.Version}\n\n{System.Environment.OSVersion}\n{CultureInfo.CurrentCulture.Name}");
        Clipboard.SetContent(dataPackage);
    }
}
