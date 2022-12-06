using NickvisionApplication.Shared.Controllers;
using NickvisionApplication.Shared.Models;
using System;

namespace NickvisionApplication.GNOME.Views;

public class PreferencesDialog : Adw.Window
{
    private readonly PreferencesViewController _controller;
    private readonly Gtk.Box _mainBox;
    private readonly Adw.HeaderBar _headerBar;
    private readonly Adw.PreferencesPage _page;
    private readonly Adw.PreferencesGroup _grpUserInterface;
    private readonly Adw.ComboRow _rowTheme;

    public PreferencesDialog(PreferencesViewController controller, Gtk.Window parent)
    {
        //Window Settings
        _controller = controller;
        New();
        SetTransientFor(parent);
        SetDefaultSize(800, 600);
        SetModal(true);
        SetDestroyWithParent(false);
        SetHideOnClose(true);
        //Main Box
        _mainBox = Gtk.Box.New(Gtk.Orientation.Vertical, 0);
        //Header Bar
        _headerBar = Adw.HeaderBar.New();
        _headerBar.SetTitleWidget(Adw.WindowTitle.New("Preferences", ""));
        _mainBox.Append(_headerBar);
        //Preferences Page
        _page = Adw.PreferencesPage.New();
        _mainBox.Append(_page);
        //User Interface Group
        _grpUserInterface = Adw.PreferencesGroup.New();
        _grpUserInterface.SetTitle("User Interface");
        _grpUserInterface.SetDescription("Customize the application's user interface.");
        _rowTheme = Adw.ComboRow.New();
        _rowTheme.SetTitle("Theme");
        _rowTheme.SetModel(Gtk.StringList.New(new string[] { "Light", "Dark", "System" }));
        _grpUserInterface.Add(_rowTheme);
        _page.Add(_grpUserInterface);
        //Layout
        SetContent(_mainBox);
        OnHide += Hide;
        //Load Config
        _rowTheme.SetSelected((uint)_controller.Theme);
    }

    private void Hide(Gtk.Widget sender, EventArgs e)
    {
        _controller.Theme = (Theme)_rowTheme.GetSelected();
        _controller.SaveConfiguration();
        Destroy();
    }
}
