using NickvisionApplication.Shared.Controllers;
using NickvisionApplication.Shared.Models;
using System;
using System.Runtime.InteropServices;

namespace NickvisionApplication.GNOME.Views;

/// <summary>
/// The PreferencesDialog for the application
/// </summary>
public class PreferencesDialog : Adw.Window
{
    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    private delegate void SignalCallback(nint gObject, nint gParamSpec, nint data);

    [DllImport("adwaita-1")]
    private static extern ulong g_signal_connect_data(nint instance, [MarshalAs(UnmanagedType.LPStr)] string detailed_signal, [MarshalAs(UnmanagedType.FunctionPtr)]SignalCallback c_handler, nint data, nint destroy_data, int connect_flags);

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
        _grpUserInterface.SetTitle(_controller.Localizer["UserInterface"]);
        _grpUserInterface.SetDescription(_controller.Localizer["UserInterfaceDescription"]);
        _rowTheme = Adw.ComboRow.New();
        _rowTheme.SetTitle(_controller.Localizer["Theme"]);
        _rowTheme.SetModel(Gtk.StringList.New(new string[] { _controller.Localizer["ThemeLight"], _controller.Localizer["ThemeDark"], _controller.Localizer["ThemeSystem"] }));
        g_signal_connect_data(_rowTheme.Handle, "notify::selected-item", OnThemeChanged, IntPtr.Zero, IntPtr.Zero, 0);
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
        _controller.SaveConfiguration();
        Destroy();
    }

    private void OnThemeChanged(nint sender, nint gParamSpec, nint data)
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
