using Microsoft.Extensions.Hosting;
using Nickvision.Application.GNOME.Views;
using Nickvision.Application.Shared.Helpers;
using Nickvision.Desktop.GNOME.Helpers;
using System.Threading.Tasks;

namespace Nickvision.Application.GNOME;

public class Program
{
    public static async Task Main(string[] args)
    {
        var newArgs = new string[args.Length + 1];
        newArgs[0] = "org.nickvision.application";
        args.CopyTo(newArgs, 1);
        var builder = Host.CreateApplicationBuilder();
        builder.ConfigureApplication(newArgs);
        builder.ConfigureAdw<MainWindow>();
        var app = builder.Build();
        await app.RunAsync();
    }
}