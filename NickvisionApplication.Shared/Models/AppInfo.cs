using System;
using System.Collections.Generic;

namespace NickvisionApplication.Shared.Models;

/// <summary>
/// A model for the information about the application
/// </summary>
public class AppInfo
{
    private static AppInfo? _instance;

    /// <summary>
    /// Gets the singleton object
    /// </summary>
    public static AppInfo Current => _instance ??= new AppInfo();

    /// <summary>
    /// The id of the application
    /// </summary>
    public string ID { get; set; }
    /// <summary>
    /// The name of the application
    /// </summary>
    public string Name { get; set; }
    /// <summary>
    /// The short name of the application
    /// </summary>
    public string ShortName { get; set; }
    /// <summary>
    /// The description of the application
    /// </summary>
    public string Description { get; set; }
    /// <summary>
    /// The running version of the application
    /// </summary>
    public string Version { get; set; }
    /// <summary>
    /// The changelog for the running version of the application
    /// </summary>
    public string Changelog { get; set; }
    /// <summary>
    /// The GitHub repo for the application
    /// </summary>
    public Uri GitHubRepo { get; set; }
    /// <summary>
    /// The issue tracker url for the application
    /// </summary>
    public Uri IssueTracker { get; set; }
    /// <summary>
    /// The support url for the application
    /// </summary>
    public Uri SupportUrl { get; set; }
    /// <summary>
    /// The list of translators of the application
    /// </summary>
    public List<string> Translators { get; init; }

    /// <summary>
    /// Constructs an AppInfo
    /// </summary>
    private AppInfo()
    {
        ID = "";
        Name = "";
        Description = "";
        ShortName = "";
        Version = "0.0.0";
        Changelog = "";
        GitHubRepo = new Uri("about:blank");
        IssueTracker = new Uri("about:blank");
        SupportUrl = new Uri("about:blank");
        Translators = new List<string>();
    }
}
