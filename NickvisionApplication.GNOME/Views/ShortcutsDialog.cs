using NickvisionApplication.Shared.Models;
using NickvisionApplication.Shared.Helpers;
using System;

namespace NickvisionApplication.GNOME.Views;

/// <summary>
/// The ShortcutsDialog for the application
/// </summary>
public class ShortcutsDialog : Gtk.ShortcutsWindow
{
    private readonly Gtk.Builder _builder;
    private readonly Gtk.ShortcutsWindow _window;

    public ShortcutsDialog(Localizer localizer, Gtk.Window parent)
    {
        string mXml = @"\
            <?xml version='1.0' encoding='UTF-8'?>\
            <interface>\
                <object class='GtkShortcutsWindow' id='mDialog'>\
                    <property name='default-width'>600</property>\
                    <property name='default-height'>500</property>\
                    <property name='modal'>true</property>\
                    <property name='resizable'>true</property>\
                    <property name='destroy-with-parent'>false</property>\
                    <property name='hide-on-close'>true</property>\
                    <child>\
                        <object class='GtkShortcutsSection'>\
                            <child>\
                                <object class='GtkShortcutsGroup'>\
                                    <property name='title'>{ localizer['Format'] }</property>\
                                    <child>\
                                        <object class='GtkShortcutsShortcut'>\
                                            <property name='title'>%s</property>\
                                            <property name='accelerator'>&lt;Control&gt;o</property>\
                                        </object>\
                                    </child>\
                                    <child>\
                                        <object class='GtkShortcutsShortcut'>\
                                            <property name='title'>%s</property>\
                                            <property name='accelerator'>&lt;Control&gt;w</property>\
                                        </object>\
                                    </child>\
                                </object>\
                            </child>\
                            <child>\
                                <object class='GtkShortcutsGroup'>\
                                    <property name='title'>%s</property>\
                                    <child>\
                                        <object class='GtkShortcutsShortcut'>\
                                            <property name='title'>%s</property>\
                                            <property name='accelerator'>&lt;Control&gt;comma</property>\
                                        </object>\
                                    </child>\
                                    <child>\
                                        <object class='GtkShortcutsShortcut'>\
                                            <property name='title'>%s</property>\
                                            <property name='accelerator'>&lt;Control&gt;question</property>\
                                        </object>\
                                    </child>\
                                    <child>\
                                        <object class='GtkShortcutsShortcut'>\
                                            <property name='title'>%s</property>\
                                            <property name='accelerator'>F1</property>\
                                        </object>\
                                    </child>\
                                </object>\
                            </child>\
                        </object>\
                    </child>\
                </object>\
            </interface>";

        _builder = Gtk.Builder.NewFromString(mXml, -1);
        _window = _builder.GetObject("mDialog");
        _window.SetTransientFor(parent);
    }
    // _("Folder"),
    // _("Open Folder"),
    // _("Close Folder"),
    // _("Application"),
    // _("Preferences"),
    // _("Keyboard Shortcuts"),
    // _("About")
    // );
    // GtkBuilder* builder{ gtk_builder_new_from_string(m_xml.c_str(), -1) };
    // m_gobj = GTK_WIDGET(gtk_builder_get_object(builder, "m_dialog"));
    // gtk_window_set_transient_for(GTK_WINDOW(m_gobj), GTK_WINDOW(parent));
}