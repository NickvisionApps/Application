#include "views/preferences_dialog.h"

using namespace application::controllers;
using namespace application::linux::helpers;
using namespace application::models;
using namespace desktop::app;

namespace application::linux::views
{
	static void adw_combo_row_set_model(AdwComboRow* row, const std::vector<std::string>& items, unsigned int selected = 0)
	{
		GtkStringList* list{ gtk_string_list_new(nullptr) };
		for (const std::string& item : items)
		{
			gtk_string_list_append(list, item.c_str());
		}
		adw_combo_row_set_model(row, G_LIST_MODEL(list));
		g_object_unref(list);
		adw_combo_row_set_selected(row, selected);
	}

	static void on_notify(GObject* /* unused */, GParamSpec* /* unused */, gpointer data)
	{
		static_cast<preferences_dialog*>(data)->apply_changes();
	}

	preferences_dialog::preferences_dialog(std::shared_ptr<preferences_view_controller> controller,
	                                       const std::shared_ptr<translation_service>& translation_service)
	    : m_controller{ std::move(controller) },
	      m_builder{ "preferences_dialog", translation_service },
	      m_dialog{ m_builder.get<AdwDialog>("root") }
	{
		g_signal_connect(m_builder.get<AdwComboRow>("theme_row"), "notify::selected-item", G_CALLBACK(on_notify), this);
		g_signal_connect(m_builder.get<AdwComboRow>("language_row"), "notify::selected-item", G_CALLBACK(on_notify), this);
		adw_combo_row_set_model(m_builder.get<AdwComboRow>("theme_row"), m_controller->get_available_themes(), m_controller->get_theme());
		adw_combo_row_set_model(m_builder.get<AdwComboRow>("language_row"), m_controller->get_available_translation_languages(),
		                        m_controller->get_translation_language());
	}

	AdwDialog* preferences_dialog::gobj() const
	{
		return m_dialog;
	}

	void preferences_dialog::present(GtkWidget* parent)
	{
		adw_dialog_present(m_dialog, parent);
	}

	void preferences_dialog::apply_changes()
	{
		switch (static_cast<theme>(adw_combo_row_get_selected(m_builder.get<AdwComboRow>("theme_row"))))
		{
		case theme::light:
			adw_style_manager_set_color_scheme(adw_style_manager_get_default(), ADW_COLOR_SCHEME_FORCE_LIGHT);
			break;
		case theme::dark:
			adw_style_manager_set_color_scheme(adw_style_manager_get_default(), ADW_COLOR_SCHEME_FORCE_DARK);
			break;
		case theme::system:
			adw_style_manager_set_color_scheme(adw_style_manager_get_default(), ADW_COLOR_SCHEME_DEFAULT);
			break;
		}
		m_controller->set_theme(adw_combo_row_get_selected(m_builder.get<AdwComboRow>("theme_row")));
		m_controller->set_translation_language(adw_combo_row_get_selected(m_builder.get<AdwComboRow>("language_row")));
	}
}