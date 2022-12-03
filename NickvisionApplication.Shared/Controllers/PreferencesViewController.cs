using NickvisionApplication.Shared.Models;

namespace NickvisionApplication.Shared.Controllers;

/// <summary>
/// A controller for a PreferencesView
/// </summary>
public class PreferencesViewController
{
    /// <summary>
    /// Constructs a PreferencesViewController
    /// </summary>
    public PreferencesViewController()
    {

    }

    /// <summary>
    /// The prefered theme of the application
    /// </summary>
    public Theme Theme
    {
        get => Configuration.Current.Theme;

        set => Configuration.Current.Theme = value;
    }

    /// <summary>
    /// Saves the configuration to disk
    /// </summary>
    public void Save() => Configuration.Current.Save();
}
