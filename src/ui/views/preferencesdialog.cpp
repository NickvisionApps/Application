#include "preferencesdialog.hpp"
#include "../../helpers/translation.hpp"

using namespace NickvisionApplication::Controllers;
using namespace NickvisionApplication::UI::Views;

PreferencesDialog::PreferencesDialog(GtkWindow* parent, const PreferencesDialogController& controller) : m_controller{ controller }, m_gobj{ adw_window_new() }
{
    //Window Settings
    gtk_window_set_transient_for(GTK_WINDOW(m_gobj), parent);
    gtk_window_set_default_size(GTK_WINDOW(m_gobj), 800, 600);
    gtk_window_set_modal(GTK_WINDOW(m_gobj), true);
    gtk_window_set_destroy_with_parent(GTK_WINDOW(m_gobj), false);
    gtk_window_set_hide_on_close(GTK_WINDOW(m_gobj), true);
    //Header Bar
    m_headerBar = adw_header_bar_new();
    adw_header_bar_set_title_widget(ADW_HEADER_BAR(m_headerBar), adw_window_title_new(_("Preferences"), nullptr));
    //User Interface Group
    m_grpUserInterface = adw_preferences_group_new();
    adw_preferences_group_set_title(ADW_PREFERENCES_GROUP(m_grpUserInterface), _("User Interface"));
    adw_preferences_group_set_description(ADW_PREFERENCES_GROUP(m_grpUserInterface), _("Customize the application's user interface."));
    //Theme Row
    m_rowTheme = adw_combo_row_new();
    adw_preferences_row_set_title(ADW_PREFERENCES_ROW(m_rowTheme), _("Theme"));
    adw_combo_row_set_model(ADW_COMBO_ROW(m_rowTheme), G_LIST_MODEL(gtk_string_list_new(new const char*[4]{ _("System"), _("Light"), _("Dark"), nullptr })));
    adw_preferences_group_add(ADW_PREFERENCES_GROUP(m_grpUserInterface), m_rowTheme);
    g_signal_connect(m_rowTheme, "notify::selected-item", G_CALLBACK((void (*)(GObject*, GParamSpec*, gpointer))[](GObject*, GParamSpec*, gpointer data) { reinterpret_cast<PreferencesDialog*>(data)->onThemeChanged(); }), this);
    //Application Group
    m_grpApplication = adw_preferences_group_new();
    adw_preferences_group_set_title(ADW_PREFERENCES_GROUP(m_grpApplication), _("Application"));
    adw_preferences_group_set_description(ADW_PREFERENCES_GROUP(m_grpApplication), _("Customize application settings."));
    //Is First Time Open Row
    m_rowIsFirstTimeOpen = adw_action_row_new();
    m_switchIsFirstTimeOpen = gtk_switch_new();
    gtk_widget_set_valign(m_switchIsFirstTimeOpen, GTK_ALIGN_CENTER);
    adw_preferences_row_set_title(ADW_PREFERENCES_ROW(m_rowIsFirstTimeOpen), _("Is First Time Open"));
    adw_action_row_add_suffix(ADW_ACTION_ROW(m_rowIsFirstTimeOpen), m_switchIsFirstTimeOpen);
    adw_action_row_set_activatable_widget(ADW_ACTION_ROW(m_rowIsFirstTimeOpen), m_switchIsFirstTimeOpen);
    adw_preferences_group_add(ADW_PREFERENCES_GROUP(m_grpApplication), m_rowIsFirstTimeOpen);
    //Page
    m_page = adw_preferences_page_new();
    adw_preferences_page_add(ADW_PREFERENCES_PAGE(m_page), ADW_PREFERENCES_GROUP(m_grpUserInterface));
    adw_preferences_page_add(ADW_PREFERENCES_PAGE(m_page), ADW_PREFERENCES_GROUP(m_grpApplication));
    //Main Box
    m_mainBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_append(GTK_BOX(m_mainBox), m_headerBar);
    gtk_box_append(GTK_BOX(m_mainBox), m_page);
    adw_window_set_content(ADW_WINDOW(m_gobj), m_mainBox);
    //Load Configuration
    adw_combo_row_set_selected(ADW_COMBO_ROW(m_rowTheme), m_controller.getThemeAsInt());
    gtk_switch_set_active(GTK_SWITCH(m_switchIsFirstTimeOpen), m_controller.getIsFirstTimeOpen());
}

GtkWidget* PreferencesDialog::gobj()
{
    return m_gobj;
}

void PreferencesDialog::run()
{
    gtk_widget_show(m_gobj);
    while(gtk_widget_is_visible(m_gobj))
    {
        g_main_context_iteration(g_main_context_default(), false);
    }
    m_controller.setIsFirstTimeOpen(gtk_switch_get_active(GTK_SWITCH(m_switchIsFirstTimeOpen)));
    m_controller.saveConfiguration();
    gtk_window_destroy(GTK_WINDOW(m_gobj));
}

void PreferencesDialog::onThemeChanged()
{
    m_controller.setTheme(adw_combo_row_get_selected(ADW_COMBO_ROW(m_rowTheme)));
    if(m_controller.getThemeAsInt() == 0)
    {
        adw_style_manager_set_color_scheme(adw_style_manager_get_default(), ADW_COLOR_SCHEME_PREFER_LIGHT);
    }
    else if(m_controller.getThemeAsInt() == 1)
    {
        adw_style_manager_set_color_scheme(adw_style_manager_get_default(), ADW_COLOR_SCHEME_FORCE_LIGHT);
    }
    else if(m_controller.getThemeAsInt() == 2)
    {
        adw_style_manager_set_color_scheme(adw_style_manager_get_default(), ADW_COLOR_SCHEME_FORCE_DARK);
    }
}
