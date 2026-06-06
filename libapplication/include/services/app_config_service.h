#pragma once

#include <libdesktop.h>
#include <memory>
#include <string>
#include <tuple>
#include "models/theme.h"

namespace application::services
{
	class app_config_service
	{
	public:
		using dependencies = std::tuple<desktop::app::configuration_service>;
		app_config_service(std::shared_ptr<desktop::app::configuration_service> config);
		~app_config_service() = default;
		app_config_service(const app_config_service&) = delete;
		app_config_service(app_config_service&&) = delete;
		bool allow_preview_updates();
		void set_allow_preview_updates(bool allow);
		models::theme get_theme();
		void set_theme(models::theme theme);
		std::string get_translation_language();
		void set_translation_language(const std::string& language);
		desktop::app::window_geometry get_window_geometry();
		void set_window_geometry(const desktop::app::window_geometry& geometry);
		app_config_service& operator=(const app_config_service&) = delete;
		app_config_service& operator=(app_config_service&&) = delete;

	private:
		std::shared_ptr<desktop::app::configuration_service> m_configuration_service;
	};
}