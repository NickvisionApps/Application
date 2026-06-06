#include "controllers/main_window_controller.h"
#include <chrono>
#include <format>
#include <optional>

using namespace application::services;
using namespace application::models;
using namespace desktop::app;
using namespace desktop::network;
using namespace desktop::notifications;
using namespace desktop::system;
using namespace desktop::updates;

namespace application::controllers
{
	main_window_controller::main_window_controller(std::shared_ptr<app_info> app_info, std::shared_ptr<app_config_service> app_config_service,
	                                               std::shared_ptr<folder_service> folder_service, std::shared_ptr<notification_service> notification_service,
	                                               std::shared_ptr<translation_service> translation_service, std::shared_ptr<update_service> update_service)
	    : m_app_info{ std::move(app_info) },
	      m_app_config_service{ std::move(app_config_service) },
	      m_folder_service{ std::move(folder_service) },
	      m_notification_service{ std::move(notification_service) },
	      m_translation_service{ std::move(translation_service) },
	      m_update_service{ std::move(update_service) }
	{
		m_app_info->set_short_name(m_translation_service->_("Application"));
		m_app_info->set_description(m_translation_service->_("Create new Nickvision application"));
		m_app_info->add_extra_link(m_translation_service->_("Matrix Chat"), "https://matrix.to/#/#nickvision:matrix.org");
		m_app_info->add_developer(m_translation_service->_("Nicholas Logozzo"), "https://githjub.com/nlogozzo");
		m_app_info->add_developer(m_translation_service->_("Contributors on GitHub ❤️"), "https://github.com/NickvisionApps/Application/graphs/contributors");
		m_app_info->add_designer(m_translation_service->_("Nicholas Logozzo"), "https://github.com/nlogozzo");
		m_app_info->add_designer(m_translation_service->_("Fyodor Sobolev"), "https://github.com/fsobolev");
		m_app_info->add_designer(m_translation_service->_("DaPigGuy"), "https://github.com/DaPigGuy");
		m_app_info->add_artist(m_translation_service->_("David Lapshin"), "https://github.com/daudix");
		m_app_info->set_translation_credits(m_translation_service->_("translation-credits"));
	}

	bool main_window_controller::can_shutdown() const
	{
		return true;
	}

	std::shared_ptr<app_info> main_window_controller::get_app_info() const
	{
		return m_app_info;
	}

	const std::filesystem::path& main_window_controller::get_current_folder() const
	{
		return m_folder_service->get_path();
	}

	std::string main_window_controller::get_greeting() const
	{
		long hours{ std::chrono::duration_cast<std::chrono::hours>(std::chrono::system_clock::now().time_since_epoch()).count() % 24 };
		if (hours >= 0 && hours < 6)
		{
			return m_translation_service->_p("Night", "Good Morning!");
		}
		if (hours < 12)
		{
			return m_translation_service->_p("Morning", "Good Morning!");
		}
		if (hours < 18)
		{
			return m_translation_service->_("Good Afternoon!");
		}
		if (hours < 24)
		{
			return m_translation_service->_("Good Evening!");
		}
		return m_translation_service->_("Good Day!");
	}

	theme main_window_controller::get_theme() const
	{
		return m_app_config_service->get_theme();
	}

	desktop::app::window_geometry main_window_controller::get_window_geometry() const
	{
		return m_app_config_service->get_window_geometry();
	}

	void main_window_controller::set_window_geometry(const desktop::app::window_geometry& geometry)
	{
		m_app_config_service->set_window_geometry(geometry);
	}

	std::string main_window_controller::get_debugging_information() const
	{
		std::string info{ std::format("{} {}\n\n", m_app_info->get_short_name(), m_app_info->get_version().str()) };
		info += environment::get_debugging_information();
		return info;
	}

	void main_window_controller::queue_check_for_updates(bool show_no_update_notification)
	{
		std::optional<version> stable{ m_update_service->get_latest_version(false) };
		if (stable.has_value())
		{
			m_latest_version = *stable;
		}
		if (m_app_config_service->allow_preview_updates())
		{
			std::optional<version> preview{ m_update_service->get_latest_version(true) };
			if (preview.has_value() && stable.has_value() && *preview > *stable)
			{
				m_latest_version = *preview;
			}
		}
		if (m_latest_version > m_app_info->get_version())
		{
#ifdef __linux__
			return;
#else
			m_notification_service->send(
			    app_notification{ m_translation_service->_("New {} update available: {}", m_app_info->get_short_name(), m_latest_version.str()),
			                      notification_severity::information, "update" });
#endif
		}
		else if (show_no_update_notification)
		{
			m_notification_service->send(app_notification{ m_translation_service->_("No Update Available"), notification_severity::warning });
		}
	}

#ifdef _WIN32
	void main_window_controller::install_update_for_windows(const std::function<void(const desktop::network::download_progress&)>& progress)
	{
		bool result{ m_update_service->install_update_for_windows(m_latest_version, progress) };
		if (result)
		{
			m_notification_service->send(
			    app_notification{ m_translation_service->_("Starting {} installer...", m_app_info->get_short_name()), notification_severity::success });
		}
		else
		{
			m_notification_service->send(
			    app_notification{ m_translation_service->_("Unable to download and install the update"), notification_severity::error });
		}
	}
#endif

	void main_window_controller::open_folder(const std::filesystem::path& path)
	{
		m_folder_service->open(path);
	}

	void main_window_controller::close_folder()
	{
		m_folder_service->close();
	}
}