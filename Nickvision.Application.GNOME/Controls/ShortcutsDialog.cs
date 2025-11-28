using Nickvision.Application.GNOME.Helpers;
using Nickvision.Desktop.Globalization;

namespace Nickvision.Application.GNOME.Controls;

public class ShortcutsDialog
{
    private readonly Gtk.Builder _builder;
    private readonly Adw.ShortcutsDialog _shortcutsDialog;

    public ShortcutsDialog(ITranslationService translator)
    {
        _builder = Gtk.Builder.NewFromBlueprint("ShortcutsDialog", translator);
        _shortcutsDialog = new Adw.ShortcutsDialog(new Adw.Internal.ShortcutsDialogHandle(_builder.GetPointer("root"), false));
    }

    public void Present(Gtk.Widget? parent) => _shortcutsDialog.Present(parent);
}
