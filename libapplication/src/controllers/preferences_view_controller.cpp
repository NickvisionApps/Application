#include "controllers/preferences_view_controller.h"
#include <ranges>

using namespace application::models;
using namespace application::services;
using namespace desktop::app;

namespace application::controllers
{
	preferences_view_controller::preferences_view_controller(std::shared_ptr<app_config_service> app_config_service,
	                                                         std::shared_ptr<translation_service> translation_service)
	    : m_app_config_service{ std::move(app_config_service) },
	      m_translation_service{ std::move(translation_service) }
	{
	}

	std::vector<std::string> preferences_view_controller::get_available_translation_languages()
	{
		std::vector<std::string> available{ m_translation_service->get_available_languages() };
		std::vector<std::string> languages{ m_translation_service->_("System"), "en_US" };
		std::ranges::sort(available);
		for (const std::string& language : available)
		{
			if (language != "en_US")
			{
				languages.push_back(language);
			}
		}
		return languages;
	}

	std::vector<std::string> preferences_view_controller::get_available_themes()
	{
		return { m_translation_service->_p("Theme", "Light"), m_translation_service->_p("Theme", "Dark"), m_translation_service->_p("Theme", "System") };
	}

	bool preferences_view_controller::allow_preview_updates()
	{
		return m_app_config_service->allow_preview_updates();
	}

	void preferences_view_controller::set_allow_preview_updates(bool allow)
	{
		m_app_config_service->set_allow_preview_updates(allow);
	}

	theme preferences_view_controller::get_theme()
	{
		return m_app_config_service->get_theme();
	}

	void preferences_view_controller::set_theme(theme theme)
	{
		m_app_config_service->set_theme(theme);
	}

	std::string preferences_view_controller::get_translation_language()
	{
		std::string language{ m_app_config_service->get_translation_language() };
		if (language.empty())
		{
			return m_translation_service->_("System");
		}
		if (language == "C")
		{
			return "en_US";
		}
		return language;
	}

	void preferences_view_controller::set_translation_language(const std::string& language)
	{
		if (language == m_translation_service->_("System"))
		{
			m_app_config_service->set_translation_language("");
			return;
		}
		if (language == "en_US")
		{
			m_app_config_service->set_translation_language("C");
			return;
		}
		m_app_config_service->set_translation_language(language);
	}
}