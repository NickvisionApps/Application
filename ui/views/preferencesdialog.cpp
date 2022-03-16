#include "preferencesdialog.h"
#include "../xmlstrings.h"

using namespace NickvisionApplication::Models;
using namespace NickvisionApplication::UI;
using namespace NickvisionApplication::UI::Views;

PreferencesDialog::PreferencesDialog(GtkWidget* parent) : m_builder(gtk_builder_new_from_string(XmlStrings::getPreferencesDialog().c_str(), -1))
{
    //==Dialog==//
    gtk_window_set_transient_for(GTK_WINDOW(gobj()), GTK_WINDOW(parent));
    //==Signals==//
    g_signal_connect(gtk_builder_get_object(m_builder, "gtk_btnSave"), "clicked", G_CALLBACK((Callback_GtkButton)[](GtkButton* button, gpointer* data) { reinterpret_cast<PreferencesDialog*>(data)->save(); }), this);
    //==Load Config==//
    if(m_configuration.getTheme() == Theme::System)
    {
        adw_combo_row_set_selected(ADW_COMBO_ROW(gtk_builder_get_object(m_builder, "adw_rowTheme")), 0);
    }
    else if(m_configuration.getTheme() == Theme::Light)
    {
        adw_combo_row_set_selected(ADW_COMBO_ROW(gtk_builder_get_object(m_builder, "adw_rowTheme")), 1);
    }
    else if(m_configuration.getTheme() == Theme::Dark)
    {
        adw_combo_row_set_selected(ADW_COMBO_ROW(gtk_builder_get_object(m_builder, "adw_rowTheme")), 2);
    }
    gtk_switch_set_active(GTK_SWITCH(gtk_builder_get_object(m_builder, "gtk_switchIsFirstTimeOpen")), m_configuration.getIsFirstTimeOpen());
}

PreferencesDialog::~PreferencesDialog()
{
    m_configuration.setTheme(static_cast<Theme>(adw_combo_row_get_selected(ADW_COMBO_ROW(gtk_builder_get_object(m_builder, "adw_rowTheme")))));
    m_configuration.setIsFirstTimeOpen(gtk_switch_get_active(GTK_SWITCH(gtk_builder_get_object(m_builder, "gtk_switchIsFirstTimeOpen"))));
    m_configuration.save();
    gtk_window_destroy(GTK_WINDOW(gobj()));
}

GtkWidget* PreferencesDialog::gobj() const
{
    return GTK_WIDGET(gtk_builder_get_object(m_builder, "gtk_preferencesDialog"));
}

void PreferencesDialog::show()
{
    gtk_widget_show(gobj());
}

void PreferencesDialog::save()
{
    gtk_widget_hide(gobj());
}
