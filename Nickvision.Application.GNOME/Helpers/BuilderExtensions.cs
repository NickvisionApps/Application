using GLib;
using Nickvision.Desktop.Globalization;
using System;
using System.IO;
using System.Linq;
using System.Xml.Linq;

namespace Nickvision.Application.GNOME.Helpers;

public static class BuilderExtensions
{
    extension(Gtk.Builder)
    {
        public static Gtk.Builder NewFromBlueprint(string name, ITranslationService translator)
        {
            var path = Path.Combine(Desktop.System.Environment.ExecutingDirectory, "ui", $"{name}.ui");
            if(!File.Exists(path))
            {
                throw new ArgumentException($"UI file not found: {path}");
            }
            var doc = XDocument.Load(path);
            var textNodes = doc.DescendantNodes().OfType<XText>().ToList();
            foreach(var node in textNodes)
            {
                var parent = node.Parent;
                if(parent is null)
                {
                    continue;
                }
                var translatableAttribute = parent.Attribute("translatable");
                if(translatableAttribute is null)
                {
                    continue;
                }
                translatableAttribute.Remove();
                var context = parent.Attribute("context")?.Value ?? string.Empty;
                var content = node.Value;
                node.Value = !string.IsNullOrEmpty(context) ? translator.GetParticular(context, content) : translator.Get(content);
            }
            var xml = doc.ToString();
            return Gtk.Builder.NewFromString(xml, -1);
        }
    }
}
