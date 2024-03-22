#include "views/preferencesdialog.h"
#include "helpers/builder.h"
#include <iostream>

using namespace Nickvision::Application::Shared::Controllers;
using namespace Nickvision::Application::Shared::Models;

namespace Nickvision::Application::GNOME::Views
{
    PreferencesDialog::PreferencesDialog(const std::shared_ptr<PreferencesViewController>& controller)
        : m_controller{ controller },
        m_builder{ BuilderHelpers::fromBlueprint("preferences_dialog") },
        m_dialog{ ADW_PREFERENCES_DIALOG(gtk_builder_get_object(m_builder, "root")) }
    {
        //Load
        adw_combo_row_set_selected(ADW_COMBO_ROW(gtk_builder_get_object(m_builder, "themeRow")), static_cast<unsigned int>(m_controller->getTheme()));
        //Signals
        g_signal_connect(m_dialog, "closed", G_CALLBACK(+[](AdwDialog*, gpointer data){ reinterpret_cast<PreferencesDialog*>(data)->onClosed(); }), this);
        g_signal_connect(gtk_builder_get_object(m_builder, "themeRow"), "notify::selected-item", G_CALLBACK(+[](GObject*, GParamSpec* pspec, gpointer data){ reinterpret_cast<PreferencesDialog*>(data)->onThemeChanged(); }), this);
    }

    const PreferencesDialog& PreferencesDialog::create(const std::shared_ptr<PreferencesViewController>& controller)
    {
        return *(new PreferencesDialog(controller));
    }

    PreferencesDialog::~PreferencesDialog()
    {
        adw_dialog_force_close(ADW_DIALOG(m_dialog));
        g_object_unref(m_builder);
    }

    void PreferencesDialog::present(GtkWindow* parent) const
    {
        adw_dialog_present(ADW_DIALOG(m_dialog), GTK_WIDGET(parent));
    }

    void PreferencesDialog::onClosed()
    {
        m_controller->saveConfiguration();
        std::cout << "closed" << std::endl;
        delete this;
    }

    void PreferencesDialog::onThemeChanged()
    {
        m_controller->setTheme(static_cast<Theme>(adw_combo_row_get_selected(ADW_COMBO_ROW(gtk_builder_get_object(m_builder, "themeRow")))));
        switch (m_controller->getTheme())
        {
        case Theme::Light:
            adw_style_manager_set_color_scheme(adw_style_manager_get_default(), ADW_COLOR_SCHEME_FORCE_LIGHT);
            break;
        case Theme::Dark:
            adw_style_manager_set_color_scheme(adw_style_manager_get_default(), ADW_COLOR_SCHEME_FORCE_DARK);
            break;
        default:
            adw_style_manager_set_color_scheme(adw_style_manager_get_default(), ADW_COLOR_SCHEME_DEFAULT);
            break;
        }
    }
}