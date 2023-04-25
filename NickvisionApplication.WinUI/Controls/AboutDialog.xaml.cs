using Microsoft.UI.Xaml;
using Microsoft.UI.Xaml.Controls;
using NickvisionApplication.Shared.Helpers;
using NickvisionApplication.Shared.Models;
using System;
using System.Globalization;
using Windows.ApplicationModel.DataTransfer;
using Windows.System;

namespace NickvisionApplication.WinUI.Controls;

/// <summary>
/// A dialog for viewing information about an app
/// </summary>
public sealed partial class AboutDialog : ContentDialog
{
    private AppInfo _appInfo;

    public AboutDialog(AppInfo appInfo, Localizer localizer)
    {
        InitializeComponent();
        _appInfo = appInfo;
        //Localize Strings
        Title = _appInfo.ShortName;
        CloseButtonText = localizer["OK"];
        CardChangelog.Header = localizer["Changelog"];
        CardGitHubRepo.Header = localizer["GitHubRepo"];
        CardReportABug.Header = localizer["ReportABug"];
        CardDiscussions.Header = localizer["Discussions"];
        InfoBar.Message = localizer["CopiedSysInfo", "WinUI"];
        //Load AppInfo
        LblDescription.Text = _appInfo.Description;
        LblVersion.Text = _appInfo.Version;
        CardLblChangelog.Header = _appInfo.Changelog;
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
        return result;
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

    /// <summary>
    /// Occurs when the github repo button is clicked
    /// </summary>
    /// <param name="sender">object</param>
    /// <param name="e">RoutedEventArgs</param>
    private async void GitHubRepo(object sender, RoutedEventArgs e) => await Launcher.LaunchUriAsync(_appInfo.GitHubRepo);

    /// <summary>
    /// Occurs when the report a bug button is clicked
    /// </summary>
    /// <param name="sender">object</param>
    /// <param name="e">RoutedEventArgs</param>
    private async void ReportABug(object sender, RoutedEventArgs e) => await Launcher.LaunchUriAsync(_appInfo.IssueTracker);

    /// <summary>
    /// Occurs when the discussions button is clicked
    /// </summary>
    /// <param name="sender">object</param>
    /// <param name="e">RoutedEventArgs</param>
    private async void Discussions(object sender, RoutedEventArgs e) => await Launcher.LaunchUriAsync(_appInfo.SupportUrl);
}
