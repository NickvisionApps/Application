using Avalonia;
using Avalonia.Controls;
using Avalonia.Interactivity;
using Avalonia.Styling;
using FluentAvalonia.UI.Controls;
using NickvisionApplication.Fluent.Helpers;
using NickvisionApplication.Shared.Helpers;
using NickvisionApplication.Shared.Models;
using System;
using System.Globalization;

namespace NickvisionApplication.Fluent.Controls;

/// <summary>
/// A dialog for viewing information about an app
/// </summary>
public partial class AboutDialog : ContentDialog, IStyleable
{
    Type IStyleable.StyleKey => typeof(ContentDialog);
    
    private readonly AppInfo _appInfo;

    /// <summary>
    /// Constructs an AboutDialog
    /// </summary>
    public AboutDialog() => InitializeComponent();

    /// <summary>
    /// Constructs an AboutDialog
    /// </summary>
    /// <param name="appInfo">AppInfo</param>
    /// <param name="localizer">Localizer</param>
    public AboutDialog(AppInfo appInfo, Localizer localizer)
    {
        InitializeComponent();
        _appInfo = appInfo;
        //Localize Strings
        Title = _appInfo.ShortName;
        CloseButtonText = localizer["OK"];
        LblChangelogHeader.Text = localizer["Changelog"];
        LblGitHubHeader.Text = localizer["GitHubRepo"];
        LblReportABug.Text = localizer["ReportABug"];
        LblDiscussions.Text = localizer["Discussions"];
        LblCreditsHeader.Text = localizer["Credits"];
        InfoBar.Message = localizer["CopiedSysInfo", "WinUI"];
        //Load AppInfo
        LblDescription.Text = _appInfo.Description;
        LblVersion.Text = _appInfo.Version;
        LblChangelog.Text = _appInfo.Changelog;
        var credits = string.Format(localizer["CreditsDialogDescription", "WinUI"], RemoveUrlFromCredits(localizer["Developers", "Credits"]), RemoveUrlFromCredits(localizer["Designers", "Credits"]), RemoveUrlFromCredits(localizer["Artists", "Credits"]), RemoveUrlFromCredits(localizer["Translators", "Credits"]));
        if (string.IsNullOrEmpty(localizer["Translators", "Credits"]))
        {
            credits = credits.Remove(credits.TrimEnd().LastIndexOf('\n'));
        }
        LblCredits.Text = credits;
    }
    
    /// <summary>
    /// Removes urls from a credit string
    /// </summary>
    /// <param name="s">The credit string</param>
    /// <returns>The credit string without the urls</returns>
    private string RemoveUrlFromCredits(string s)
    {
        var credits = s.Split('\n');
        var result = "";
        for (int i = 0; i < credits.Length; i++)
        {
            if (credits[i].IndexOf("https://") != -1)
            {
                result += credits[i].Remove(credits[i].IndexOf("https://"));
            }
            else if (credits[i].IndexOf("http://") != -1)
            {
                result += credits[i].Remove(credits[i].IndexOf("http://"));
            }
            if (i != credits.Length - 1)
            {
                result += "\n";
            }
        }
        return string.IsNullOrEmpty(result) ? s : result;
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
        if (Application.Current!.Clipboard != null)
        {
            await Application.Current!.Clipboard.SetTextAsync($"{_appInfo.ID}\n{_appInfo.Version}\n\n{Environment.OSVersion}\n{CultureInfo.CurrentCulture.Name}");
            InfoBar.IsOpen = true;    
        }
    }

    /// <summary>
    /// Occurs when the github repo button is clicked
    /// </summary>
    /// <param name="sender">object</param>
    /// <param name="e">RoutedEventArgs</param>
    private void GitHubRepo(object sender, RoutedEventArgs e) => _appInfo.GitHubRepo.OpenInBrowser();
    
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