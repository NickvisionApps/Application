using Microsoft.UI.Xaml.Controls;
using NickvisionApplication.Shared.Models;

namespace NickvisionApplication.WinUI.Controls;

public sealed partial class AboutDialog : ContentDialog
{
    private readonly AppInfo _appInfo;

    public AboutDialog(AppInfo appInfo)
    {
        InitializeComponent();
        _appInfo = appInfo;
    }
}
