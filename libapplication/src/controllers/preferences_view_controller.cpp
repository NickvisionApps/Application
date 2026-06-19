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
	      m_translation_service{ std::move(translation_service) },
	      m_themes{ { theme::light, m_translation_service->_p("Theme", "Light") },
		            { theme::dark, m_translation_service->_p("Theme", "Dark") },
		            { theme::system, m_translation_service->_p("Theme", "System") } }
	{
		std::vector<std::string> available{ m_translation_service->get_available_languages() };
		m_translation_languages.reserve(available.size() + 2);
		m_translation_languages.emplace_back("", m_translation_service->_("System"));
		m_translation_languages.emplace_back("C", "en_US");
		std::ranges::sort(available);
		for (const std::string& language : available)
		{
			if (language != "en_US")
			{
				m_translation_languages.emplace_back(language, language);
			}
		}
	}

	std::vector<std::string> preferences_view_controller::get_available_translation_languages()
	{
		std::vector<std::string> languages;
		languages.reserve(m_translation_languages.size());
		for (const std::pair<std::string, std::string>& pair : m_translation_languages)
		{
			languages.push_back(pair.second);
		}
		return languages;
	}

	std::vector<std::string> preferences_view_controller::get_available_themes()
	{
		std::vector<std::string> themes;
		themes.reserve(m_themes.size());
		for (const std::pair<models::theme, std::string>& pair : m_themes)
		{
			themes.push_back(pair.second);
		}
		return themes;
	}

	bool preferences_view_controller::allow_preview_updates()
	{
		return m_app_config_service->allow_preview_updates();
	}

	void preferences_view_controller::set_allow_preview_updates(bool allow)
	{
		m_app_config_service->set_allow_preview_updates(allow);
	}

	unsigned int preferences_view_controller::get_theme()
	{
		for (size_t i{ 0 }; i < m_themes.size(); ++i)
		{
			if (m_themes[i].first == m_app_config_service->get_theme())
			{
				return static_cast<unsigned int>(i);
			}
		}
		return 0;
	}

	void preferences_view_controller::set_theme(unsigned int index)
	{
		if (index >= m_themes.size())
		{
			return;
		}
		m_app_config_service->set_theme(m_themes[index].first);
	}

	unsigned int preferences_view_controller::get_translation_language()
	{
		for (size_t i{ 0 }; i < m_translation_languages.size(); ++i)
		{
			if (m_translation_languages[i].first == m_app_config_service->get_translation_language())
			{
				return static_cast<unsigned int>(i);
			}
		}
		return 0;
	}

	void preferences_view_controller::set_translation_language(unsigned int index)
	{
		if (index >= m_translation_languages.size())
		{
			return;
		}
		m_app_config_service->set_translation_language(m_translation_languages[index].first);
	}
}