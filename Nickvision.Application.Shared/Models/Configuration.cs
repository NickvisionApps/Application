using Nickvision.Desktop.Application;

namespace Nickvision.Application.Shared.Models;

public class Configuration
{
    public static readonly string Key;

    public bool AllowPreviewUpdates { get; set; }
    public Theme Theme { get; set; }
    public string TranslationLanguage { get; set; }
    public WindowGeometry WindowGeometry { get; set; }

    static Configuration()
    {
        Key = "config";
    }

    public Configuration()
    {
        Theme = Theme.System;
        TranslationLanguage = string.Empty;
        WindowGeometry = new WindowGeometry();
        AllowPreviewUpdates = false;
    }
}
