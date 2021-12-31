#include "settingsdialog.h"


namespace NickvisionApplication::Views
{
    SettingsDialog::SettingsDialog(Gtk::Window& parent) : Gtk::Dialog("Settings", parent, true, true)
    {
        //==Settings==//
        set_default_size(600, 500);
        set_resizable(false);
        set_hide_on_close(true);
        //==General Section==//
        Gtk::Label lblGeneral("General", Gtk::Align::START);
        lblGeneral.set_markup("<b>General</b>");
        lblGeneral.set_margin_start(20);
        lblGeneral.set_margin_top(20);
        m_chkIsFirstTimeOpen.set_label("Is First Time Open");
        m_chkIsFirstTimeOpen.set_margin_start(24);
        //==Layout==//
        Gtk::ScrolledWindow m_scroll;
        Gtk::Box mainBox(Gtk::Orientation::VERTICAL);
        mainBox.append(lblGeneral);
        mainBox.append(m_chkIsFirstTimeOpen);
        m_scroll.set_child(mainBox);
        set_child(m_scroll);
        //==Load Configuration==//
        m_chkIsFirstTimeOpen.set_active(m_configuration.isFirstTimeOpen());
    }

    SettingsDialog::~SettingsDialog()
    {
        m_configuration.setIsFirstTimeOpen(m_chkIsFirstTimeOpen.get_active());
        m_configuration.save();
    }
}
