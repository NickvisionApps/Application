using Microsoft.UI.Xaml;
using Microsoft.UI.Xaml.Controls;
using NickvisionApplication.Shared.Helpers;
using NickvisionApplication.Shared.Models;
using System;
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
        Title = string.Format(localizer["About"], _appInfo.ShortName);
        CloseButtonText = localizer["OK"];
        //Load AppInfo
        LblVersion.Text = $"V{_appInfo.Version}";
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
    /// Occurs when the github repo menu item is clicked
    /// </summary>
    /// <param name="sender">object</param>
    /// <param name="e">RoutedEventArgs</param>
    private async void GitHubRepo(object sender, RoutedEventArgs e) => await Launcher.LaunchUriAsync(_appInfo.GitHubRepo);

    /// <summary>
    /// Occurs when the report a bug menu item is clicked
    /// </summary>
    /// <param name="sender">object</param>
    /// <param name="e">RoutedEventArgs</param>
    private async void ReportABug(object sender, RoutedEventArgs e) => await Launcher.LaunchUriAsync(_appInfo.IssueTracker);

    /// <summary>
    /// Occurs when the discussions menu item is clicked
    /// </summary>
    /// <param name="sender">object</param>
    /// <param name="e">RoutedEventArgs</param>
    private async void Discussions(object sender, RoutedEventArgs e) => await Launcher.LaunchUriAsync(_appInfo.SupportUrl);
}
