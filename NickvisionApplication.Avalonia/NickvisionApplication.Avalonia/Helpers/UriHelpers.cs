using System;
using System.Diagnostics;

namespace NickvisionApplication.Avalonia.Helpers;

/// <summary>
/// Helper methods for working with Uri
/// </summary>
public static class UriHelpers
{
    /// <summary>
    /// Opens a Uri link in the browser
    /// </summary>
    /// <param name="uri">The uri to open</param>
    public static void OpenInBrowser(this Uri uri)
    {
        if (OperatingSystem.IsWindows())
        {
            var url = uri.ToString().Replace("&", "^&");
            Process.Start(new ProcessStartInfo(url) { UseShellExecute = true });
        }
        else if (OperatingSystem.IsLinux())
        {
            Process.Start("xdg-open", uri.ToString());
        }
        else if (OperatingSystem.IsMacOS())
        {
            Process.Start("open", uri.ToString());
        }
        else if (OperatingSystem.IsAndroid())
        {
            Process.Start(new ProcessStartInfo(uri.ToString()) { UseShellExecute = true, Verb = "open" });
        }
    }
}