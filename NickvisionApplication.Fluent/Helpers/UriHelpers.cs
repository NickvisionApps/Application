using System;
using System.Diagnostics;
using System.Runtime.InteropServices;

namespace NickvisionApplication.Fluent.Helpers;

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
        if (RuntimeInformation.IsOSPlatform(OSPlatform.Windows))
        {
            var url = uri.ToString().Replace("&", "^&");
            Process.Start(new ProcessStartInfo(url) { UseShellExecute = true });
        }
        else if (RuntimeInformation.IsOSPlatform(OSPlatform.Linux))
        {
            Process.Start("xdg-open", uri.ToString());
        }
        else if (RuntimeInformation.IsOSPlatform(OSPlatform.OSX))
        {
            Process.Start("open", uri.ToString());
        }
    }
}