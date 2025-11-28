using System.Collections.Generic;

namespace Nickvision.Application.GNOME.Helpers;

public static class ComboRowExtensions
{
    extension(Adw.ComboRow row)
    {
        public void SetModel(List<string> strs, string selected, bool allowElipse = true)
        {
            var selectedIndex = 0u;
            var list = Gtk.StringList.New(null);
            for (var i = 0; i < strs.Count; i++)
            {
                var str = strs[i];
                list.Append(str);
                if (str == selected)
                {
                    selectedIndex = (uint)i;
                }
            }
            if (!allowElipse)
            {
                (row.Factory! as Gtk.SignalListItemFactory)!.OnSetup += (sender, args) =>
                {
                    var box = (args.Object as Gtk.ListItem)!.Child;
                    var label = box!.GetFirstChild() as Gtk.Label;
                    label!.Ellipsize = Pango.EllipsizeMode.None;
                };
            }
            row.Model = list;
            list.Unref();
            row.Selected = selectedIndex;
        }

        public void SetModel(List<string> strs, uint selected, bool allowElipse = true)
        {
            var list = Gtk.StringList.New(null);
            foreach (var str in strs)
            {
                list.Append(str);
            }
            if (!allowElipse)
            {
                (row.Factory! as Gtk.SignalListItemFactory)!.OnSetup += (sender, args) =>
                {
                    var box = (args.Object as Gtk.ListItem)!.Child;
                    var label = box!.GetFirstChild() as Gtk.Label;
                    label!.Ellipsize = Pango.EllipsizeMode.None;
                };
            }
            row.Model = list;
            list.Unref();
            row.Selected = selected < strs.Count ? selected : 0;
        }
    }
}
