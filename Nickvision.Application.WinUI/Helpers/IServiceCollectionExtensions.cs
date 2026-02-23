using Microsoft.Extensions.DependencyInjection;
using Nickvision.Application.WinUI.Controls;
using Nickvision.Application.WinUI.Views;

namespace Nickvision.Application.WinUI.Helpers;

public static class IServiceCollectionExtensions
{
    extension(IServiceCollection collection)
    {
        public IServiceCollection AddControls()
        {
            collection.AddSingleton<MainWindow>();
            collection.AddTransient<SettingsPage>();
            collection.AddTransient<AboutDialog>();
            return collection;
        }
    }
}