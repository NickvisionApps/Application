using Nickvision.Application.Shared.Controllers;
using Nickvision.Application.Shared.Models;
using Nickvision.Desktop.GNOME.Helpers;
using System;

namespace Nickvision.Application.GNOME.Views;

public class PreferencesDialog : Adw.PreferencesDialog
{
    private readonly PreferencesViewController _controller;
    private readonly Gtk.Builder _builder;

    [Gtk.Connect("themeRow")]
    private Adw.ComboRow? _themeRow;
    [Gtk.Connect("languageRow")]
    private Adw.ComboRow? _languageRow;

    public PreferencesDialog(PreferencesViewController controller) : this(controller, Gtk.Builder.NewFromBlueprint("PreferencesDialog", controller.Translator))
    {

    }

    private PreferencesDialog(PreferencesViewController controller, Gtk.Builder builder) : base(new Adw.Internal.PreferencesDialogHandle(builder.GetPointer("root"), false))
    {
        _controller = controller;
        _builder = builder;
        _builder.Connect(this);
        // Load
        _themeRow!.SetModel(_controller.Themes);
        _languageRow!.SetModel(_controller.AvailableTranslationLanguages);
        // Events
        OnClosed += Dialog_OnClosed;
        _themeRow!.OnNotify += ThemeRow_OnNotify;
    }

    private async void Dialog_OnClosed(Adw.Dialog sender, EventArgs args)
    {
        _controller.TranslationLanguage = _controller.AvailableTranslationLanguages[(int)_languageRow!.Selected];
        await _controller.SaveConfigurationAsync();
    }

    private void ThemeRow_OnNotify(GObject.Object sender, NotifySignalArgs args)
    {
        if (args.Pspec.GetName() == "selected-item")
        {
            _controller.Theme = _controller.Themes[(int)_themeRow!.Selected];
            Adw.StyleManager.GetDefault().ColorScheme = _controller.Themes[(int)_themeRow!.Selected].Value switch
            {
                Theme.Light => Adw.ColorScheme.ForceLight,
                Theme.Dark => Adw.ColorScheme.ForceDark,
                _ => Adw.ColorScheme.Default
            };
        }
    }
}
