using Nickvision.Aura;
using NickvisionApplication.Shared.Models;

namespace NickvisionApplication.Shared.Controllers;

/// <summary>
/// A controller for a PreferencesView
/// </summary>
public class PreferencesViewController
{
    /// <summary>
    /// Gets the AppInfo object
    /// </summary>
    public AppInfo AppInfo => Aura.Active.AppInfo;

    /// <summary>
    /// Constructs a PreferencesViewController
    /// </summary>
    internal PreferencesViewController()
    {

    }

    /// <summary>
    /// The preferred theme of the application
    /// </summary>
    public Theme Theme
    {
        get => Configuration.Current.Theme;

        set => Configuration.Current.Theme = value;
    }

    /// <summary>
    /// Whether or not to automatically check for updates
    /// </summary>
    public bool AutomaticallyCheckForUpdates
    {
        get => Configuration.Current.AutomaticallyCheckForUpdates;

        set => Configuration.Current.AutomaticallyCheckForUpdates = value;
    }

    /// <summary>
    /// Saves the configuration to disk
    /// </summary>
    public void SaveConfiguration() => Aura.Active.SaveConfig("config");
}
