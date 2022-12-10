using NickvisionApplication.Shared.Models;
using NickvisionApplication.Shared.Helpers;
using System;

namespace NickvisionApplication.GNOME.Views;

/// <summary>
/// The ShortcutsDialog for the application
/// </summary>
public class ShortcutsDialog
{
    private readonly Gtk.Builder _builder;
    private readonly Gtk.ShortcutsWindow _window;

    public ShortcutsDialog(Localizer localizer, Gtk.Window parent)
    {
    	string mXml = $@"<?xml version='1.0' encoding='UTF-8'?>
            <interface>
                <object class='GtkShortcutsWindow' id='mDialog'>
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
                                    <property name='title'>{ localizer["ShortcutsFolder"] }</property>
                                    <child>
                                        <object class='GtkShortcutsShortcut'>
                                            <property name='title'>{ localizer["ShortcutsOpenFolder"] }</property>
                                            <property name='accelerator'>&lt;Control&gt;o</property>
                                        </object>
                                    </child>
                                    <child>
                                        <object class='GtkShortcutsShortcut'>
                                            <property name='title'>{ localizer["ShortcutsCloseFolder"] }</property>
                                            <property name='accelerator'>&lt;Control&gt;w</property>
                                        </object>
                                    </child>
                                </object>
                            </child>
                            <child>
                                <object class='GtkShortcutsGroup'>
                                    <property name='title'>{ localizer["ShortcutsApplication"] }</property>
                                    <child>
                                        <object class='GtkShortcutsShortcut'>
                                            <property name='title'>{ localizer["ShortcutsPreferences"] }</property>
                                            <property name='accelerator'>&lt;Control&gt;comma</property>
                                        </object>
                                    </child>
                                    <child>
                                        <object class='GtkShortcutsShortcut'>
                                            <property name='title'>{ localizer["ShortcutsShortcuts"] }</property>
                                            <property name='accelerator'>&lt;Control&gt;question</property>
                                        </object>
                                    </child>
                                    <child>
                                        <object class='GtkShortcutsShortcut'>
                                            <property name='title'>{ localizer["ShortcutsAbout"] }</property>
                                            <property name='accelerator'>F1</property>
                                        </object>
                                    </child>
                                </object>
                            </child>
                        </object>
                    </child>
                </object>
            </interface>";

        _builder = Gtk.Builder.NewFromString(mXml, -1);
        _window = (Gtk.ShortcutsWindow)_builder.GetObject("mDialog");
        _window.SetTransientFor(parent);
        _window.Show();
    }
}