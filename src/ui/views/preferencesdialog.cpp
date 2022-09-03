#include "preferencesdialog.hpp"

using namespace NickvisionApplication::UI::Views;

PreferencesDialog::PreferencesDialog(GtkWindow* parent) : m_gobj{ adw_window_new() }
{
    //Window Settings
    gtk_window_set_transient_for(GTK_WINDOW(m_gobj), parent);
    gtk_window_set_default_size(GTK_WINDOW(m_gobj), 800, 600);
    gtk_window_set_modal(GTK_WINDOW(m_gobj), true);
    gtk_window_set_deletable(GTK_WINDOW(m_gobj), false);
    gtk_window_set_destroy_with_parent(GTK_WINDOW(m_gobj), false);
    gtk_window_set_hide_on_close(GTK_WINDOW(m_gobj), true);
    //Header Bar
    m_headerBar = adw_header_bar_new();
    adw_header_bar_set_title_widget(ADW_HEADER_BAR(m_headerBar), adw_window_title_new("Preferences", nullptr));
    //Cancel Button
    m_btnCancel = gtk_button_new();
    gtk_button_set_label(GTK_BUTTON(m_btnCancel), "Cancel");
    g_signal_connect(m_btnCancel, "clicked", G_CALLBACK((void (*)(GtkButton*, gpointer*))[](GtkButton*, gpointer* data) { reinterpret_cast<PreferencesDialog*>(data)->onCancel(); }), this);
    adw_header_bar_pack_start(ADW_HEADER_BAR(m_headerBar), m_btnCancel);
    //Save Button
    m_btnSave = gtk_button_new();
    gtk_button_set_label(GTK_BUTTON(m_btnSave), "Save");
    gtk_style_context_add_class(gtk_widget_get_style_context(m_btnSave), "suggested-action");
    g_signal_connect(m_btnSave, "clicked", G_CALLBACK((void (*)(GtkButton*, gpointer*))[](GtkButton*, gpointer* data) { reinterpret_cast<PreferencesDialog*>(data)->onSave(); }), this);
    adw_header_bar_pack_end(ADW_HEADER_BAR(m_headerBar), m_btnSave);
    //Main Box
    m_mainBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_append(GTK_BOX(m_mainBox), m_headerBar);
    adw_window_set_content(ADW_WINDOW(m_gobj), m_mainBox);
}

PreferencesDialog::~PreferencesDialog()
{
    gtk_window_destroy(GTK_WINDOW(m_gobj));
}

GtkWidget* PreferencesDialog::gobj() const
{
    return m_gobj;
}

void PreferencesDialog::show()
{
    gtk_widget_show(m_gobj);
}

void PreferencesDialog::onCancel()
{
    gtk_widget_hide(m_gobj);
}

void PreferencesDialog::onSave()
{
    gtk_widget_hide(m_gobj);
}

void PreferencesDialog::onRowIsFirstTimeOpenActivate()
{

}
