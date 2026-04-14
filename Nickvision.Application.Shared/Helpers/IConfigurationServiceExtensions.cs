using Nickvision.Application.Shared.Models;
using Nickvision.Desktop.Application;

namespace Nickvision.Application.Shared.Helpers;

public static class IConfigurationServiceExtensions
{
    extension(IConfigurationService configurationService)
    {
        public bool AllowPreviewUpdates
        {
            get => configurationService.Get("AllowPreviewUpdates", false);

            set => configurationService.Set("AllowPreviewUpdates", value);
        }

        public Theme Theme
        {
            get => configurationService.Get("Theme", Theme.System);

            set => configurationService.Set("Theme", value);
        }

        public string TranslationLanguage
        {
            get => configurationService.Get("TranslationLanguage", string.Empty);

            set => configurationService.Set("TranslationLanguage", value);
        }

        public WindowGeometry WindowGeometry
        {
            get => configurationService.Get("WindowGeometry", new WindowGeometry(), ApplicationJsonContext.Default.WindowGeometry);

            set => configurationService.Set("WindowGeometry", value, ApplicationJsonContext.Default.WindowGeometry);
        }
    }
}
