﻿using Nickvision.Aura;
using NickvisionApplication.Shared.Events;
using NickvisionApplication.Shared.Models;
using System;
using System.IO;
using static NickvisionApplication.Shared.Helpers.Gettext;

namespace NickvisionApplication.Shared.Controllers;

/// <summary>
/// A controller for a MainWindow
/// </summary>
public class MainWindowController
{
    /// <summary>
    /// Applciation's Aura
    /// </summary>
    public Aura Aura { get; init; }
    /// <summary>
    /// Gets the AppInfo object
    /// </summary>
    public AppInfo AppInfo => Aura.Active.AppInfo;
    /// <summary>
    /// The path of the folder opened
    /// </summary>
    public string FolderPath { get; private set; }
    /// <summary>
    /// The preferred theme of the application
    /// </summary>
    public Theme Theme => Configuration.Current.Theme;
    /// <summary>
    /// Whether or not the folder is opened
    /// </summary>
    public bool IsFolderOpened => !string.IsNullOrEmpty(FolderPath);

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
    
    public MainWindowController()
    {
        Aura = new Aura("org.nickvision.application", "Nickvision Application", _("Application"), _("Create new Nickvision applications"));
        Aura.Active.SetConfig<Configuration>("config");
        AppInfo.Version = "2023.8.0-next";
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
    }

    /// <summary>
    /// Open the folder (if exists) at startup
    /// </summary>
    /// <param name="args">Command-line arguments</param>
    /// <remarks>Expected to be called after the main window started</remarks>
    public void Startup(string[] args)
    {
        if(args.Length > 0 && Directory.Exists(args[0]))
        {
            OpenFolder(args[0]);
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
    /// Creates a new PreferencesViewController
    /// </summary>
    /// <returns>The PreferencesViewController</returns>
    public PreferencesViewController CreatePreferencesViewController() => new PreferencesViewController();

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
    }
}
