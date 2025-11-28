using Nickvision.Desktop.System;
using System.IO;

namespace Nickvision.Application.GNOME.Helpers;

public static class BuilderExtensions
{
    extension(Gtk.Builder)
    {
        public static Gtk.Builder NewFromBlueprint(string name) => Gtk.Builder.NewFromFile(Path.Combine(Environment.ExecutingDirectory, "ui", $"{name}.ui"));
    }
}
