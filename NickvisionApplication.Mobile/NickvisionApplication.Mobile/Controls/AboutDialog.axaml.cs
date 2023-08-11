using Avalonia;
using Avalonia.Controls;
using Avalonia.Interactivity;
using Avalonia.Styling;
using FluentAvalonia.UI.Controls;
using Nickvision.Aura;
using NickvisionApplication.Mobile.Helpers;
using System;
using System.Globalization;
using static NickvisionApplication.Shared.Helpers.Gettext;

namespace NickvisionApplication.Mobile.Controls;

public partial class AboutDialog : ContentDialog, IStyleable
{
    Type IStyleable.StyleKey => typeof(ContentDialog);
    
    private readonly AppInfo _appInfo;
    
    /// <summary>
    /// Constructs an AboutDialog
    /// </summary>
    /// <param name="appInfo">AppInfo</param>
    /// <param name="localizer">Localizer</param>
    public AboutDialog(AppInfo appInfo)
    {
        InitializeComponent();
        _appInfo = appInfo;
        //Localize Strings
        Title = _appInfo.ShortName;
        CloseButtonText = _("OK");
        LblChangelogHeader.Text = _("Changelog");
        LblGitHubHeader.Text = _("GitHub Repo");
        LblReportABug.Text = _("Report a Bug");
        LblDiscussions.Text = _("Discussions");
        LblCreditsHeader.Text = _("Credits");
        InfoBar.Message = _("Copied system information.");
        //Load AppInfo
        LblDescription.Text = _appInfo.Description;
        LblVersion.Text = _appInfo.Version;
        LblChangelog.Text = _appInfo.Changelog;
        var credits = string.Format(_("Developers:\n{0}\n\nDesigners:\n{1}\n\nArtists:\n{2}\n\nTranslators:\n{3}"),
            string.Join('\n', _appInfo.Developers.Keys), string.Join('\n', _appInfo.Designers.Keys),
            string.Join('\n', _appInfo.Artists.Keys), _appInfo.TranslatorCredits);
        if (string.IsNullOrEmpty(_appInfo.TranslatorCredits))
        {
            credits = credits.Remove(credits.TrimEnd().LastIndexOf('\n'));
        }
        LblCredits.Text = credits;
    }
    
    /// <summary>
    /// Occurs when the ScrollViewer's scrolling is changed
    /// </summary>
    /// <param name="sender"></param>
    /// <param name="e"></param>
    private void ScrollViewer_ScrollChanged(object? sender, ScrollChangedEventArgs e) => StackPanel.Margin = new Thickness(0, 0, ScrollViewer.Extent.Height > ScrollViewer.Viewport.Height ? 14 : 0, 0);

    /// <summary>
    /// Occurs when the version button is clicked
    /// </summary>
    /// <param name="sender">object?</param>
    /// <param name="e">RoutedEventArgs</param>
    private async void CopySystemInformation(object? sender, RoutedEventArgs e)
    {
        if (TopLevel.GetTopLevel(this) != null && TopLevel.GetTopLevel(this)!.Clipboard != null)
        {
            await TopLevel.GetTopLevel(this)!.Clipboard!.SetTextAsync($"{_appInfo.ID}\n{_appInfo.Version}\n\n{Environment.OSVersion}\n{CultureInfo.CurrentCulture.Name}");
            InfoBar.IsOpen = true;    
        }
    }

    /// <summary>
    /// Occurs when the github repo button is clicked
    /// </summary>
    /// <param name="sender">object</param>
    /// <param name="e">RoutedEventArgs</param>
    private void GitHubRepo(object sender, RoutedEventArgs e) => _appInfo.SourceRepo.OpenInBrowser();
    
    /// <summary>
    /// Occurs when the report a bug button is clicked
    /// </summary>
    /// <param name="sender">object</param>
    /// <param name="e">RoutedEventArgs</param>
    private void ReportABug(object sender, RoutedEventArgs e) => _appInfo.IssueTracker.OpenInBrowser();
    
    /// <summary>
    /// Occurs when the discussions button is clicked
    /// </summary>
    /// <param name="sender">object</param>
    /// <param name="e">RoutedEventArgs</param>
    private void Discussions(object sender, RoutedEventArgs e) => _appInfo.SupportUrl.OpenInBrowser();
}