using Microsoft.Extensions.Hosting;
using Nickvision.Application.Shared.Helpers;
using Nickvision.Desktop.WinUI.Helpers;
using System;
using System.Runtime.InteropServices;

namespace Nickvision.Application.WinUI;

public static partial class Program
{
    [LibraryImport("Microsoft.ui.xaml.dll", EntryPoint = "XamlCheckProcessRequirements")]
    private static partial void XamlCheckProcessRequirements();

    [STAThread]
    private static void Main(string[] args)
    {
        XamlCheckProcessRequirements();
        var builder = Host.CreateApplicationBuilder();
        builder.ConfigureApplication(args);
        builder.ConfigureWinUI<App>();
        var host = builder.Build();
        host.Run();
    }
}
