using Microsoft.Extensions.Logging;
using Nickvision.Application.Shared.Helpers;
using Nickvision.Application.Shared.Models;
using Nickvision.Application.Shared.Services;
using Nickvision.Desktop.Application;
using Nickvision.Desktop.Globalization;
using Nickvision.Desktop.Network;
using Nickvision.Desktop.Notifications;
using System;
using System.Threading.Tasks;

namespace Nickvision.Application.Shared.Controllers;

public class MainWindowController
{
    ILogger<MainWindowController> _logger;
    private readonly AppInfo _appInfo;
    private readonly IConfigurationService _configurationService;
    private readonly IFolderService _folderService;
    private readonly INotificationService _notificationService;
    private readonly ITranslationService _translationService;
    private readonly IUpdaterService _updaterService;
    private AppVersion _latestVersion;

    public MainWindowController(ILogger<MainWindowController> logger, AppInfo appInfo, IArgumentsService argumentsService, IConfigurationService configurationService, IFolderService folderService, INotificationService notificationService, ITranslationService translationService, IUpdaterService updaterService)
    {
        _logger = logger;
        _appInfo = appInfo;
        _configurationService = configurationService;
        _folderService = folderService;
        _notificationService = notificationService;
        _translationService = translationService;
        _updaterService = updaterService;
        _latestVersion = appInfo.Version!;
        _translationService.Language = _configurationService.TranslationLanguage;
        _logger.LogInformation($"Received command-line arguments: [{string.Join(", ", argumentsService.Data)}]");
        // Translate strings
        _appInfo.ShortName = _translationService._("Application");
        _appInfo.Description = _translationService._("Create new Nickvision applications.");
        _appInfo.ExtraLinks.Add(_translationService._("Matrix Chat"), new Uri("https://matrix.to/#/#nickvision:matrix.org"));
        _appInfo.Developers.Add(_translationService._("Nicholas Logozzo"), "https://github.com/nlogozzo");
        _appInfo.Developers.Add(_translationService._("Contributors on GitHub ❤️"), "https://github.com/NickvisionApps/Application/graphs/contributors");
        _appInfo.Designers.Add(_translationService._("Nicholas Logozzo"), "https://github.com/nlogozzo");
        _appInfo.Designers.Add(_translationService._("Fyodor Sobolev"), "https://github.com/fsobolev");
        _appInfo.Designers.Add(_translationService._("DaPigGuy"), "https://github.com/DaPigGuy");
        _appInfo.Artists.Add(_translationService._("David Lapshin"), "https://github.com/daudix");
        _appInfo.TranslationCredits = _translationService._("translation-credits");
    }

    public bool CanShutdown => true;

    public string? CurrentFolder => _folderService.Path;

    public string Greeting => DateTime.Now.Hour switch
    {
        >= 0 and < 6 => _translationService._p("Night", "Good Morning!"),
        < 12 => _translationService._p("Morning", "Good Morning!"),
        < 18 => _translationService._("Good Afternoon!"),
        < 24 => _translationService._("Good Evening!"),
        var _ => _translationService._("Good Day!")
    };

    public Theme Theme => _configurationService.Theme;

    public WindowGeometry WindowGeometry
    {
        get => _configurationService.WindowGeometry;

        set
        {
            _configurationService.WindowGeometry = value;
            _configurationService.Save();
        }
    }

    public async Task CheckForUpdatesAsync(bool showNotificationForNoUpdates)
    {
        _logger.LogInformation("Checking for updates...");
        var stableVersion = await _updaterService.GetLatestStableVersionAsync();
        if (stableVersion is not null)
        {
            _latestVersion = stableVersion;
        }
        if (_configurationService.AllowPreviewUpdates)
        {
            var previewVersion = await _updaterService.GetLatestPreviewVersionAsync();
            if (previewVersion is not null && previewVersion > stableVersion)
            {
                _latestVersion = previewVersion;
            }
        }
        if (_latestVersion > _appInfo.Version!)
        {
            if (!OperatingSystem.IsLinux())
            {
                _logger.LogInformation($"New application update available: {_latestVersion}");
                _notificationService.Send(new AppNotification(_translationService._("New {0} update available: {1}", _appInfo.ShortName!, _latestVersion.ToString()), NotificationSeverity.Success)
                {
                    Action = "update"
                });
            }
        }
        else
        {
            _logger.LogInformation("No application update available.");
            if (showNotificationForNoUpdates)
            {
                _notificationService.Send(new AppNotification(_translationService._("No update available"), NotificationSeverity.Warning));
            }
        }
    }

    public string GetDebugInformation(string extraInformation = "") => Desktop.System.Environment.GetDebugInformation(_appInfo, extraInformation);

    public void OpenFolder(string path) => _folderService.Open(path);

    public void CloseFolder() => _folderService.Close();

    public async Task WindowsUpdateAsync(IProgress<DownloadProgress> progress)
    {
        var res = await _updaterService.WindowsApplicationUpdateAsync(_latestVersion, progress);
        if (res)
        {
            _notificationService.Send(new AppNotification(_translationService._("Starting {0} installer...", _appInfo.ShortName!), NotificationSeverity.Success));
        }
        else
        {
            _notificationService.Send(new AppNotification(_translationService._("Unable to download and install the update"), NotificationSeverity.Error));
        }
    }
}
