using NickvisionApplication.Shared.Controllers;
using NickvisionApplication.Shared.Models;
using System;

namespace NickvisionApplication.GNOME.Views;

/// <summary>
/// The PreferencesDialog for the application
/// </summary>
public partial class PreferencesDialog : Adw.PreferencesWindow
{
    private readonly PreferencesViewController _controller;
    private readonly Adw.Application _application;

    [Gtk.Connect] private readonly Gtk.Box _mainBox;
    [Gtk.Connect] private readonly Adw.ComboRow _themeRow;

    /// <summary>
    /// Constructs a PreferencesDialog
    /// </summary>
    /// <param name="controller">PreferencesViewController</param>
    /// <param name="application">Adw.Application</param>
    /// <param name="parent">Gtk.Window</param>
    public PreferencesDialog(PreferencesViewController controller, Adw.Application application, Gtk.Window parent)
    {
        //Window Settings
        _controller = controller;
        _application = application;
        SetTransientFor(parent);
        SetDefaultSize(600, 400);
        SetModal(true);
        SetDestroyWithParent(false);
        SetHideOnClose(true);
        //Build UI
        var builder = Builder.FromFile("preferences_dialog.ui", _controller.Localizer);
        _themeRow.OnNotify += (sender, e) =>
        {
            if (e.Pspec.GetName() == "selected-item")
            {
                OnThemeChanged();
            }
        };
        _themeRow.SetSelected((uint)_controller.Theme);
        //Layout
        SetContent(_mainBox);
        OnHide += Hide;
    }

    /// <summary>
    /// Occurs when the dialog is hidden
    /// </summary>
    /// <param name="sender">Gtk.Widget</param>
    /// <param name="e">EventArgs</param>
    private void Hide(Gtk.Widget sender, EventArgs e)
    {
        _controller.SaveConfiguration();
        Destroy();
    }

    /// <summary>
    /// Occurs when the theme selection is changed
    /// </summary>
    private void OnThemeChanged()
    {
        _controller.Theme = (Theme)_rowTheme.GetSelected();
        _application.StyleManager!.ColorScheme = _controller.Theme switch
        {
            Theme.System => Adw.ColorScheme.PreferLight,
            Theme.Light => Adw.ColorScheme.ForceLight,
            Theme.Dark => Adw.ColorScheme.ForceDark,
            _ => Adw.ColorScheme.PreferLight
        };
    }
}
