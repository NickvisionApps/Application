#pragma once

#include <libdesktop.h>
#include <memory>
#include <string>
#include <tuple>
#include <utility>
#include <vector>
#include "models/theme.h"
#include "services/app_config_service.h"

namespace application::controllers
{
	class preferences_view_controller
	{
	public:
		using dependencies = std::tuple<services::app_config_service, desktop::app::translation_service>;
		preferences_view_controller(std::shared_ptr<services::app_config_service> app_config_service,
		                            std::shared_ptr<desktop::app::translation_service> translation_service);
		~preferences_view_controller() = default;
		preferences_view_controller(const preferences_view_controller&) = delete;
		preferences_view_controller(preferences_view_controller&&) = delete;
		std::vector<std::string> get_available_translation_languages();
		std::vector<std::string> get_available_themes();
		bool allow_preview_updates();
		void set_allow_preview_updates(bool allow);
		unsigned int get_theme();
		void set_theme(unsigned int index);
		unsigned int get_translation_language();
		void set_translation_language(unsigned int index);
		preferences_view_controller& operator=(const preferences_view_controller&) = delete;
		preferences_view_controller& operator=(preferences_view_controller&&) = delete;

	private:
		std::shared_ptr<services::app_config_service> m_app_config_service;
		std::shared_ptr<desktop::app::translation_service> m_translation_service;
		std::vector<std::pair<models::theme, std::string>> m_themes;
		std::vector<std::pair<std::string, std::string>> m_translation_languages;
	};
}