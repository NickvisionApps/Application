#pragma once

#include <libdesktop.h>
#include <memory>
#include <string>
#include <tuple>
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
		models::theme get_theme();
		void set_theme(models::theme theme);
		std::string get_translation_language();
		void set_translation_language(const std::string& language);
		preferences_view_controller& operator=(const preferences_view_controller&) = delete;
		preferences_view_controller& operator=(preferences_view_controller&&) = delete;

	private:
		std::shared_ptr<services::app_config_service> m_app_config_service;
		std::shared_ptr<desktop::app::translation_service> m_translation_service;
	};
}