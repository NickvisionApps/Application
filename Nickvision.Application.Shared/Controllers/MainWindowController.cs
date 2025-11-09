using Nickvision.Application.Shared.Models;
using Nickvision.Desktop.Application;
using Nickvision.Desktop.Filesystem;
using Nickvision.Desktop.Globalization;
using Nickvision.Desktop.Notifications;
using System;
using System.Globalization;
using System.Net.Http;

namespace Nickvision.Application.Shared.Controllers;

public class MainWindowController : IDisposable
{
    private readonly string[] _args;
    private readonly HttpClient _httpClient;
    private readonly ServiceCollection _services;

    public AppInfo AppInfo { get; init; }

    public MainWindowController(string[] args)
    {
        _args = args;
        _services = new ServiceCollection();
        _httpClient = new HttpClient();
        AppInfo = new AppInfo("org.nickvision.application", "Nickvision Application", "Application")
        {
            Version = new AppVersion("2025.11.0-next"),
            Changelog = "- Initial release",
            SourceRepository = new Uri("https://github.com/NickvisionApps/Application"),
            IssueTracker = new Uri("https://github.com/NickvisionApps/Application/issues/new"),
            DiscussionsForum = new Uri("https://github.com/NickvisionApps/Application/discussions")
        };
        // Register services
        var jsonFileService = _services.Add<IJsonFileService>(new JsonFileService(AppInfo));
        _services.Add<IUpdaterService>(new GitHubUpdaterService(AppInfo, _httpClient));
        var translationService = _services.Add<ITranslationService>(new GettextTranslationService(AppInfo, jsonFileService!.Load<Configuration>("config").TranslationLanguage));
        _services.Add<INotificationService>(new NotificationService(AppInfo, translationService!._("Open")));
        // Translate strings
        AppInfo.ShortName = translationService._("Application");
        AppInfo.Description = translationService._("Create new Nickvision applications.");
        AppInfo.ExtraLinks.Add(translationService._("Matrix Chat"), new Uri("https://matrix.to/#/#nickvision:matrix.org"));
        AppInfo.Developers.Add("Nicholas Logozzo", "https://github.com/nlogozzo");
        AppInfo.Developers.Add(translationService._("Contributors on GitHub ❤️"), "https://github.com/NickvisionApps/Application/graphs/contributors");
        AppInfo.Designers.Add("Nicholas Logozzo", "https://github.com/nlogozzo");
        AppInfo.Designers.Add(translationService._("Fyodor Sobolev"), "https://github.com/fsobolev");
        AppInfo.Designers.Add("DaPigGuy", "https://github.com/DaPigGuy");
        AppInfo.Artists.Add(translationService._("David Lapshin"), "https://github.com/daudix");
        AppInfo.TranslationCredits = translationService._("translation-credits");
    }

    ~MainWindowController()
    {
        Dispose(false);
    }

    public event EventHandler<AppNotificationSentEventArgs>? AppNotificationSent
    {
        add => _services.Get<INotificationService>()!.AppNotificationSent += value;

        remove => _services.Get<INotificationService>()!.AppNotificationSent -= value;
    }

    public event EventHandler<JsonFileSavedEventArgs>? JsonFileSaved
    {
        add => _services.Get<IJsonFileService>()!.Saved += value;

        remove => _services.Get<IJsonFileService>()!.Saved -= value;
    }

    public bool AllowPreviewUpdates
    {
        get => _services.Get<IJsonFileService>()!.Load<Configuration>("config").AllowPreviewUpdates;

        set
        {
            var config = _services.Get<IJsonFileService>()!.Load<Configuration>("config");
            config.AllowPreviewUpdates = value;
            _services.Get<IJsonFileService>()!.Save(config, "config");
        }
    }

    public bool CanShutdown => true;

    public string Greeting =>
        DateTime.Now.Hour switch
        {
            >= 0 and < 6 => _services.Get<ITranslationService>()!._p("Night", "Good Morning!"),
            < 12 => _services.Get<ITranslationService>()!._p("Morning", "Good Morning!"),
            < 18 => _services.Get<ITranslationService>()!._("Good Afternoon!"),
            < 24 => _services.Get<ITranslationService>()!._("Good Evening!"),
            var _ => _services.Get<ITranslationService>()!._("Good Day!")
        };

    public Theme Theme
    {
        get => _services.Get<IJsonFileService>()!.Load<Configuration>("config").Theme;

        set
        {
            var config = _services.Get<IJsonFileService>()!.Load<Configuration>("config");
            config.Theme = value;
            _services.Get<IJsonFileService>()!.Save(config, "config");
        }
    }

    public WindowGeometry WindowGeometry
    {
        get => _services.Get<IJsonFileService>()!.Load<Configuration>("config").WindowGeometry;

        set
        {
            var config = _services.Get<IJsonFileService>()!.Load<Configuration>("config");
            config.WindowGeometry = value;
            _services.Get<IJsonFileService>()!.Save(config, "config");
        }
    }

    public void Dispose()
    {
        Dispose(true);
        GC.SuppressFinalize(this);
    }

    public string GetDebugInformation(string extraInformation) => Desktop.System.Environment.GetDebugInformation(AppInfo, $"{CultureInfo.CurrentCulture}\n{extraInformation}");

    private void Dispose(bool disposing)
    {
        if (!disposing)
        {
            return;
        }
        _services.Dispose();
        _httpClient.Dispose();
    }
}
