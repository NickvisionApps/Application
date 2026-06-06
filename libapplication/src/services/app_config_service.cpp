#include "services/app_config_service.h"

using namespace application::models;
using namespace desktop::app;

namespace application::services
{
	app_config_service::app_config_service(std::shared_ptr<configuration_service> config)
	    : m_configuration_service{ std::move(config) }
	{
	}

	bool app_config_service::allow_preview_updates()
	{
		return m_configuration_service->get("allow_preview_updates", false);
	}

	void app_config_service::set_allow_preview_updates(bool allow)
	{
		m_configuration_service->set("allow_preview_updates", allow);
	}

	theme app_config_service::get_theme()
	{
		return m_configuration_service->get("theme", theme::system);
	}

	void app_config_service::set_theme(theme theme)
	{
		m_configuration_service->set("theme", theme);
	}

	std::string app_config_service::get_translation_language()
	{
		return m_configuration_service->get<std::string>("translation_language", "");
	}

	void app_config_service::set_translation_language(const std::string& language)
	{
		m_configuration_service->set("translation_language", language);
	}

	window_geometry app_config_service::get_window_geometry()
	{
		return m_configuration_service->get<window_geometry>("window_geometry", {});
	}

	void app_config_service::set_window_geometry(const window_geometry& geometry)
	{
		m_configuration_service->set("window_geometry", geometry);
	}
}