using NickvisionApplication.Shared.Events;
using NickvisionApplication.Shared.Models;
using System;
using System.IO;

namespace NickvisionApplication.Shared.Controllers;

/// <summary>
/// A controller for a MainWindow
/// </summary>
public class MainWindowController
{
    private bool _isOpened;

    /// <summary>
    /// Gets the AppInfo object
    /// </summary>
    public AppInfo AppInfo => AppInfo.Current;
    /// <summary>
    /// Whether or not the version is a development version or not
    /// </summary>
    public bool IsDevVersion => AppInfo.Current.Version.IndexOf('-') != -1;
    /// <summary>
    /// The path of the folder opened
    /// </summary>
    public string FolderPath { get; private set; }

    /// <summary>
    /// Occurs when a notification is sent
    /// </summary>
    public event EventHandler<NotificationSentEventArgs>? NotificationSent;
    /// <summary>
    /// Occurs when a folder is opened or closed
    /// </summary>
    public event EventHandler? FolderChanged;

    /// <summary>
    /// Constructs a MainWindowController
    /// </summary>
    public MainWindowController()
    {
        _isOpened = false;
        FolderPath = "No Folder Opened";
    }

    /// <summary>
    /// Runs startup functions
    /// </summary>
    public void Startup()
    {
        if(!_isOpened)
        {
            _isOpened = false;
        }
    }

    /// <summary>
    /// Opens a folder
    /// </summary>
    /// <param name="folderPath">The path of the folder to open</param>
    /// <returns>True if folder opened, else false</returns>
    public bool OpenFolder(string folderPath)
    {
        if(Directory.Exists(folderPath))
        {
            FolderPath = folderPath;
            NotificationSent?.Invoke(this, new NotificationSentEventArgs($"Folder Opened: {FolderPath}", NotificationSeverity.Success));
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
        FolderPath = "No Folder Opened";
        NotificationSent?.Invoke(this, new NotificationSentEventArgs("Folder closed.", NotificationSeverity.Warning));
        FolderChanged?.Invoke(this, EventArgs.Empty);
    }
}
