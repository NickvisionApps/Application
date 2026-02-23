using Microsoft.Extensions.DependencyInjection;
using Nickvision.Application.GNOME.Views;

namespace Nickvision.Application.GNOME.Helpers;

public static class IServiceCollectionExtensions
{
    extension(IServiceCollection collection)
    {
        public IServiceCollection AddControls()
        {
            collection.AddTransient<PreferencesDialog>();
            return collection;
        }
    }
}