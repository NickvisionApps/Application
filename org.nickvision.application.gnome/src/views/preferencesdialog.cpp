#include "views/preferencesdialog.h"
#include "helpers/gtkhelpers.h"

using namespace Nickvision::Application::Shared::Controllers;
using namespace Nickvision::Application::Shared::Models;
using namespace Nickvision::Application::GNOME::Helpers;
using namespace Nickvision::Events;

namespace Nickvision::Application::GNOME::Views
{
    PreferencesDialog::PreferencesDialog(const std::shared_ptr<PreferencesViewController>& controller, GtkWindow* parent)
        : DialogBase{ parent, "preferences_dialog" },
        m_controller{ controller }
    {
        //Load
        adw_combo_row_set_selected(m_builder.get<AdwComboRow>("themeRow"), static_cast<unsigned int>(m_controller->getTheme()));
        GtkHelpers::setComboRowModel(m_builder.get<AdwComboRow>("languageRow"), m_controller->getAvailableTranslationLanguages(), m_controller->getTranslationLanguage());
        //Signals
        m_closed += [&](const EventArgs&){ onClosed(); };
        g_signal_connect(m_builder.get<GObject>("themeRow"), "notify::selected-item", G_CALLBACK(+[](GObject*, GParamSpec* pspec, gpointer data){ reinterpret_cast<PreferencesDialog*>(data)->onThemeChanged(); }), this);
    }

    void PreferencesDialog::onClosed()
    {
        m_controller->setTranslationLanguage(static_cast<size_t>(adw_combo_row_get_selected(m_builder.get<AdwComboRow>("languageRow"))));
        m_controller->saveConfiguration();
    }

    void PreferencesDialog::onThemeChanged()
    {
        m_controller->setTheme(static_cast<Theme>(adw_combo_row_get_selected(m_builder.get<AdwComboRow>("themeRow"))));
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
