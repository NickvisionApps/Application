using NickvisionApplication.Shared.Controllers;
using NickvisionApplication.Shared.Models;
using System;

namespace NickvisionApplication.GNOME.Views;

/// <summary>
/// The PreferencesDialog for the application
/// </summary>
public class PreferencesDialog : Adw.Window
{
    private readonly PreferencesViewController _controller;
    private readonly Adw.Application _application;
    private readonly Gtk.Box _mainBox;
    private readonly Adw.HeaderBar _headerBar;
    private readonly Adw.PreferencesPage _page;
    private readonly Adw.PreferencesGroup _grpUserInterface;
    private readonly Adw.ComboRow _rowTheme;

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
        New();
        SetTransientFor(parent);
        SetDefaultSize(600, 400);
        SetModal(true);
        SetDestroyWithParent(false);
        SetHideOnClose(true);
        //Main Box
        _mainBox = Gtk.Box.New(Gtk.Orientation.Vertical, 0);
        //Header Bar
        _headerBar = Adw.HeaderBar.New();
        _headerBar.SetTitleWidget(Adw.WindowTitle.New(_controller.Localizer["Preferences"], ""));
        _mainBox.Append(_headerBar);
        //Preferences Page
        _page = Adw.PreferencesPage.New();
        _mainBox.Append(_page);
        //User Interface Group
        _grpUserInterface = Adw.PreferencesGroup.New();
        _grpUserInterface.SetTitle(_controller.Localizer["SettingsUserInterface"]);
        _grpUserInterface.SetDescription(_controller.Localizer["SettingsUserInterfaceDescription"]);
        _rowTheme = Adw.ComboRow.New();
        _rowTheme.SetTitle(_controller.Localizer["SettingsTheme"]);
        _rowTheme.SetSubtitle(_controller.Localizer["SettingThemeDescriptionGtk"]);
        _rowTheme.SetModel(Gtk.StringList.New(new string[] { _controller.Localizer["SettingsThemeLight"], _controller.Localizer["SettingsThemeDark"], _controller.Localizer["SettingsThemeSystem"] }));
        _grpUserInterface.Add(_rowTheme);
        _page.Add(_grpUserInterface);
        //Layout
        SetContent(_mainBox);
        OnHide += Hide;
        //Load Config
        _rowTheme.SetSelected((uint)_controller.Theme);
    }

    /// <summary>
    /// Occurs when the dialog is hidden
    /// </summary>
    /// <param name="sender">Gtk.Widget</param>
    /// <param name="e">EventArgs</param>
    private void Hide(Gtk.Widget sender, EventArgs e)
    {
        _controller.Theme = (Theme)_rowTheme.GetSelected();
        _application.StyleManager!.ColorScheme = _controller.Theme switch
        {
            Theme.System => Adw.ColorScheme.PreferLight,
            Theme.Light => Adw.ColorScheme.ForceLight,
            Theme.Dark => Adw.ColorScheme.ForceDark,
            _ => Adw.ColorScheme.PreferLight
        };
        _controller.SaveConfiguration();
        Destroy();
    }
}
