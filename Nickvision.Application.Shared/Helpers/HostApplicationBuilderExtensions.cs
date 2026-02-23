using Microsoft.Extensions.DependencyInjection;
using Microsoft.Extensions.Hosting;
using Nickvision.Application.Shared.Controllers;
using Nickvision.Application.Shared.Services;
using Nickvision.Desktop.Application;
using Nickvision.Desktop.Helpers;
using System;

namespace Nickvision.Application.Shared.Helpers;

public static class HostApplicationBuilderExtensions
{
    extension(IHostApplicationBuilder builder)
    {
        public IHostApplicationBuilder ConfigureApplication(string[] args)
        {
            var appInfo = new AppInfo("org.nickvision.application", "Nickvision Application", "Application")
            {
                Version = new AppVersion("2026.2.0-next"),
                Changelog = "- Initial release",
                SourceRepository = new Uri("https://github.com/NickvisionApps/Application"),
                IssueTracker = new Uri("https://github.com/NickvisionApps/Application/issues/new"),
                DiscussionsForum = new Uri("https://github.com/NickvisionApps/Application/discussions"),
                IsPortable = OperatingSystem.IsWindows() && args.Contains("--portable")
            };
            builder.Properties.Add("AppInfo", appInfo);
            builder.Services.AddSingleton(appInfo);
            builder.ConfigureNickvision(args);
            builder.Services.AddSingleton<IFolderService, FolderService>();
            builder.Services.AddSingleton<MainWindowController>();
            builder.Services.AddSingleton<PreferencesViewController>();
            return builder;
        }
    }
}
