using NickvisionApplication.Shared.Controllers;
using NickvisionApplication.Shared.Models;
using System;

namespace NickvisionApplication.GNOME.Views;

/// <summary>
/// The ShortcutsDialog for the application
/// </summary>
public class ShortcutsDialog
{
    private readonly Gtk.Builder _builder;
    private readonly Gtk.ShortcutsWindow _window;

    public ShortcutsDialog(MainWindowController controller, Gtk.Window parent)
    {
    	string xml = $@"<?xml version='1.0' encoding='UTF-8'?>
            <interface>
                <object class='GtkShortcutsWindow' id='dialog'>
                    <property name='default-width'>600</property>
                    <property name='default-height'>500</property>
                    <property name='modal'>true</property>
                    <property name='resizable'>true</property>
                    <property name='destroy-with-parent'>false</property>
                    <property name='hide-on-close'>true</property>
                    <child>
                        <object class='GtkShortcutsSection'>
                            <child>
                                <object class='GtkShortcutsGroup'>
                                    <property name='title'>{ controller.Localizer["Folder"] }</property>
                                    <child>
                                        <object class='GtkShortcutsShortcut'>
                                            <property name='title'>{ controller.Localizer["OpenFolder"] }</property>
                                            <property name='accelerator'>&lt;Control&gt;o</property>
                                        </object>
                                    </child>
                                    <child>
                                        <object class='GtkShortcutsShortcut'>
                                            <property name='title'>{ controller.Localizer["CloseFolder"] }</property>
                                            <property name='accelerator'>&lt;Control&gt;w</property>
                                        </object>
                                    </child>
                                </object>
                            </child>
                            <child>
                                <object class='GtkShortcutsGroup'>
                                    <property name='title'>{ controller.Localizer["Application"] }</property>
                                    <child>
                                        <object class='GtkShortcutsShortcut'>
                                            <property name='title'>{ controller.Localizer["Preferences"] }</property>
                                            <property name='accelerator'>&lt;Control&gt;comma</property>
                                        </object>
                                    </child>
                                    <child>
                                        <object class='GtkShortcutsShortcut'>
                                            <property name='title'>{ controller.Localizer["KeyboardShortcuts"] }</property>
                                            <property name='accelerator'>&lt;Control&gt;question</property>
                                        </object>
                                    </child>
                                    <child>
                                        <object class='GtkShortcutsShortcut'>
                                            <property name='title'>{ string.Format(controller.Localizer["About"], controller.AppInfo.ShortName) }</property>
                                            <property name='accelerator'>F1</property>
                                        </object>
                                    </child>
                                </object>
                            </child>
                        </object>
                    </child>
                </object>
            </interface>";

        _builder = Gtk.Builder.NewFromString(xml, -1);
        _window = (Gtk.ShortcutsWindow)_builder.GetObject("dialog");
        _window.SetTransientFor(parent);
    }

    public void Show() => _window.Show();
}
