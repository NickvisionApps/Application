using Nickvision.Aura;
using Nickvision.Aura.Keyring;
using Nickvision.Aura.Taskbar;
using Nickvision.Aura.Update;
using NickvisionApplication.Shared.Events;
using NickvisionApplication.Shared.Models;
using System;
using System.IO;
using System.Runtime.InteropServices;
using System.Threading.Tasks;
using static NickvisionApplication.Shared.Helpers.Gettext;

namespace NickvisionApplication.Shared.Controllers;

/// <summary>
/// A controller for a MainWindow
/// </summary>
public class MainWindowController : IDisposable
{
    private bool _disposed;
    private TaskbarItem? _taskbarItem;
    private Updater? _updater;

    /// <summary>
    /// The path of the folder opened
    /// </summary>
    public string FolderPath { get; private set; }

    /// <summary>
    /// Gets the AppInfo object
    /// </summary>
    public AppInfo AppInfo => Aura.Active.AppInfo;
    /// <summary>
    /// The preferred theme of the application
    /// </summary>
    public Theme Theme => Configuration.Current.Theme;
    /// <summary>
    /// Whether or not the folder is opened
    /// </summary>
    public bool IsFolderOpened => Directory.Exists(FolderPath);
    /// <summary>
    /// The number of files in an opened folder
    /// </summary>
    /// <remarks>If no folder opened, value will be -1</remarks>
    public int FilesCount => IsFolderOpened ? Directory.GetFiles(FolderPath, "*", SearchOption.TopDirectoryOnly).Length : -1;

    /// <summary>
    /// Occurs when a notification is sent
    /// </summary>
    public event EventHandler<NotificationSentEventArgs>? NotificationSent;
    /// <summary>
    /// Occurs when a shell notification is sent
    /// </summary>
    public event EventHandler<ShellNotificationSentEventArgs>? ShellNotificationSent;
    /// <summary>
    /// Occurs when a folder is opened or closed
    /// </summary>
    public event EventHandler<EventArgs>? FolderChanged;

    /// <summary>
    /// Constructs a MainWindowController
    /// </summary>
    /// <param name="args">Command-line arguments</param>
    public MainWindowController(string[] args)
    {
        _disposed = false;
        Aura.Init("org.nickvision.application", "Nickvision Application");
        Aura.Active.SetConfig<Configuration>("config");
        AppInfo.Version = "2023.9.0-next";
        AppInfo.ShortName = _("Application");
        AppInfo.Description = _("Create new Nickvision applications");
        AppInfo.SourceRepo = new Uri("https://github.com/NickvisionApps/Application");
        AppInfo.IssueTracker = new Uri("https://github.com/NickvisionApps/Application/issues/new");
        AppInfo.SupportUrl = new Uri("https://github.com/NickvisionApps/Application/discussions");
        AppInfo.ExtraLinks[_("Matrix Chat")] = new Uri("https://matrix.to/#/#nickvision:matrix.org");
        AppInfo.Developers[_("Nicholas Logozzo")] = new Uri("https://github.com/nlogozzo");
        AppInfo.Developers[_("Contributors on GitHub ❤️")] = new Uri("https://github.com/NickvisionApps/Application/graphs/contributors");
        AppInfo.Designers[_("Nicholas Logozzo")] = new Uri("https://github.com/nlogozzo");
        AppInfo.Designers[_("Fyodor Sobolev")] = new Uri("https://github.com/fsobolev");
        AppInfo.Designers[_("DaPigGuy")] = new Uri("https://github.com/DaPigGuy");
        AppInfo.Artists[_("David Lapshin")] = new Uri("https://github.com/daudix-UFO");
        AppInfo.TranslatorCredits = _("translator-credits");
        FolderPath = args.Length > 0 ? args[0] : "";
    }

    /// <summary>
    /// Finalizes the MainWindowController
    /// </summary>
    ~MainWindowController() => Dispose(false);

    /// <summary>
    /// The TaskbarItem
    /// </summary>
    public TaskbarItem? TaskbarItem
    {
        set
        {
            if (value == null)
            {
                return;
            }
            _taskbarItem = value;
        }
    }

    /// <summary>
    /// The string for greeting on the home page
    /// </summary>
    public string Greeting
    {
        get
        {
            return DateTime.Now.Hour switch
            {
                >= 0 and < 6 => _p("Night", "Good Morning!"),
                < 12 => _p("Morning", "Good Morning!"),
                < 18 => _("Good Afternoon!"),
                < 24 => _("Good Evening!"),
                _ => _("Good Day!")
            };
        }
    }

    /// <summary>
    /// Frees resources used by the MainWindowController object
    /// </summary>
    public void Dispose()
    {
        Dispose(true);
        GC.SuppressFinalize(this);
    }

    /// <summary>
    /// Frees resources used by the MainWindowController object
    /// </summary>
    protected virtual void Dispose(bool disposing)
    {
        if (_disposed)
        {
            return;
        }
        _taskbarItem?.Dispose();
        _disposed = true;
    }

    /// <summary>
    /// Creates a new PreferencesViewController
    /// </summary>
    /// <returns>The PreferencesViewController</returns>
    public PreferencesViewController CreatePreferencesViewController() => new PreferencesViewController();

    /// <summary>
    /// Starts the application
    /// </summary>
    /// <remarks>Expected to be called after the main window started</remarks>
    public async Task StartupAsync()
    {
        if(RuntimeInformation.IsOSPlatform(OSPlatform.Windows) && Configuration.Current.AutomaticallyCheckForUpdates)
        {
            await CheckForUpdatesAsync(); 
        }
        if (Directory.Exists(FolderPath))
        {
            OpenFolder(FolderPath);
        }
    }

    /// <summary>
    /// Checks for an application update and notifies the user if one is available
    /// </summary>
    public async Task CheckForUpdatesAsync()
    {
        if(!AppInfo.IsDevVersion)
        {
            if (_updater == null)
            {
                _updater = await Updater.NewAsync();
            }
            var version = await _updater!.GetCurrentStableVersionAsync();
            if (version != null && version > new Version(AppInfo.Version))
            {
                NotificationSent?.Invoke(this, new NotificationSentEventArgs(_("New update available."), NotificationSeverity.Success, "update"));
            }
        }
    }

    /// <summary>
    /// Downloads and installs the latest application update for Windows systems
    /// </summary>
    /// <returns>True if successful, else false</returns>
    /// <remarks>CheckForUpdatesAsync must be called before this method</remarks>
    public async Task<bool> WindowsUpdateAsync()
    {
        if(RuntimeInformation.IsOSPlatform(OSPlatform.Windows) && _updater != null)
        {
            var res = await _updater.WindowsUpdateAsync(VersionType.Stable);
            if (!res)
            {
                NotificationSent?.Invoke(this, new NotificationSentEventArgs(_("Unable to download and install update."), NotificationSeverity.Error));
            }
            return res;
        }
        return false;
    }

    /// <summary>
    /// Opens a folder
    /// </summary>
    /// <param name="folderPath">The path of the folder to open</param>
    /// <returns>True if folder opened, else false</returns>
    public bool OpenFolder(string folderPath)
    {
        if (Directory.Exists(folderPath))
        {
            FolderPath = folderPath;
            NotificationSent?.Invoke(this, new NotificationSentEventArgs(_("Folder Opened: {0}", FolderPath), NotificationSeverity.Success, "close"));
            FolderChanged?.Invoke(this, EventArgs.Empty);
            if (_taskbarItem != null)
            {
                _taskbarItem.Count = FilesCount;
            }
            return true;
        }
        return false;
    }

    /// <summary>
    /// Closes the folder
    /// </summary>
    public void CloseFolder()
    {
        FolderPath = "";
        NotificationSent?.Invoke(this, new NotificationSentEventArgs(_("Folder closed."), NotificationSeverity.Warning));
        FolderChanged?.Invoke(this, EventArgs.Empty);
        if(_taskbarItem != null)
        {
            _taskbarItem.CountVisible = false;
        }
    }
}
