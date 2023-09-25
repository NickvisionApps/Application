using Microsoft.UI.Xaml;
using Microsoft.UI.Xaml.Controls;
using Nickvision.Aura;
using System.Globalization;
using Windows.ApplicationModel.DataTransfer;
using static NickvisionApplication.Shared.Helpers.Gettext;

namespace NickvisionApplication.WinUI.Controls;

/// <summary>
/// A dialog for viewing information about an app
/// </summary>
public sealed partial class AboutDialog : ContentDialog
{
    private AppInfo _appInfo;

    /// <summary>
    /// Constructs an AboutDialog
    /// </summary>
    public AboutDialog(AppInfo appInfo)
    {
        InitializeComponent();
        _appInfo = appInfo;
        //Localize Strings
        Title = _appInfo.ShortName;
        CloseButtonText = _("OK");
        LblChangelogTitle.Text = _("Changelog");
        LblCreditsTitle.Text = _("Credits");
        InfoBar.Message = _("Copied debug info to clipboard.");
        //Load AppInfo
        LblDescription.Text = _appInfo.Description;
        LblVersion.Text = _appInfo.Version;
        LblChangelog.Text = _appInfo.Changelog;
        var credits = _("Developers:\n{0}\n\nDesigners:\n{1}\n\nArtists:\n{2}\n\nTranslators:\n{3}", string.Join("\n", _appInfo.Developers.Keys), string.Join("\n", _appInfo.Designers.Keys), string.Join("\n", _appInfo.Artists.Keys), _appInfo.TranslatorCredits == "translator-credits" ? "" : _appInfo.TranslatorCredits);
        if (_appInfo.TranslatorCredits == "translator-credits")
        {
            credits = credits.Remove(credits.TrimEnd().LastIndexOf('\n'));
        }
        LblCredits.Text = credits;
    }

    /// <summary>
    /// Occurs when the ScrollViewer's size is changed
    /// </summary>
    /// <param name="sender">object</param>
    /// <param name="e">SizeChangedEventArgs</param>
    private void ScrollViewer_SizeChanged(object sender, SizeChangedEventArgs e) => StackPanel.Margin = new Thickness(0, 0, ScrollViewer.ComputedVerticalScrollBarVisibility == Visibility.Visible ? 14 : 0, 0);

    /// <summary>
    /// Occurs when the version button is clicked
    /// </summary>
    /// <param name="sender">object</param>
    /// <param name="e">RoutedEventArgs</param>
    private void CopySystemInformation(object sender, RoutedEventArgs e)
    {
        var dataPackage = new DataPackage();
        dataPackage.SetText($"{_appInfo.ID}\n{_appInfo.Version}\n\n{System.Environment.OSVersion}\n{CultureInfo.CurrentCulture.Name}");
        Clipboard.SetContent(dataPackage);
        InfoBar.IsOpen = true;
    }
}
