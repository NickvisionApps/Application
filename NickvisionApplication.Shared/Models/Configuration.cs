using Nickvision.Aura;
using Nickvision.Aura.Configuration;
using System.Runtime.InteropServices;

namespace NickvisionApplication.Shared.Models;

/// <summary>
/// A model for the configuration of the application
/// </summary>
public class Configuration : ConfigurationBase
{
    /// <summary>
    /// The preferred theme for the application
    /// </summary>
    public Theme Theme { get; set; }
    /// <summary>
    /// Whether or not to automatically check for updates
    /// </summary>
    public bool AutomaticallyCheckForUpdates { get; set; }

    /// <summary>
    /// Constructs a Configuration
    /// </summary>
    public Configuration()
    {
        Theme = Theme.System;
        AutomaticallyCheckForUpdates = RuntimeInformation.IsOSPlatform(OSPlatform.Windows);
    }

    /// <summary>
    /// Gets the singleton object
    /// </summary>
    internal static Configuration Current => (Configuration)Aura.Active.ConfigFiles["config"];
}
