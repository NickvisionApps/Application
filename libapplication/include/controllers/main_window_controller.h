#pragma once

#include <filesystem>
#include <libdesktop.h>
#include <memory>
#include <string>
#include <tuple>
#include "models/theme.h"
#include "services/app_config_service.h"
#include "services/folder_service.h"

namespace application::controllers
{
	class main_window_controller
	{
	public:
		using dependencies = std::tuple<desktop::app::app_info, services::app_config_service, services::folder_service,
		                                desktop::notifications::notification_service, desktop::app::translation_service, desktop::updates::update_service>;
		main_window_controller(std::shared_ptr<desktop::app::app_info> app_info, std::shared_ptr<services::app_config_service> app_config_service,
		                       std::shared_ptr<services::folder_service> folder_service,
		                       std::shared_ptr<desktop::notifications::notification_service> notification_service,
		                       std::shared_ptr<desktop::app::translation_service> translation_service,
		                       std::shared_ptr<desktop::updates::update_service> update_service);
		~main_window_controller() = default;
		main_window_controller(const main_window_controller&) = delete;
		main_window_controller(main_window_controller&&) = delete;
		bool can_shutdown() const;
		const std::filesystem::path& get_current_folder() const;
		std::string get_greeting() const;
		models::theme get_theme() const;
		desktop::app::window_geometry get_window_geometry() const;
		void set_window_geometry(const desktop::app::window_geometry& geometry);
		std::string get_debugging_information(const std::string& extra) const;
		void queue_check_for_updates(bool show_no_update_notification);
#ifdef _WIN32
		void install_update_for_windows(const std::function<void(const desktop::network::download_progress&)>& progress = {});
#endif
		void open_folder(const std::filesystem::path& path);
		void close_folder();
		main_window_controller& operator=(const main_window_controller&) = delete;
		main_window_controller& operator=(main_window_controller&&) = delete;

	private:
		std::shared_ptr<desktop::app::app_info> m_app_info;
		std::shared_ptr<services::app_config_service> m_app_config_service;
		std::shared_ptr<services::folder_service> m_folder_service;
		std::shared_ptr<desktop::notifications::notification_service> m_notification_service;
		std::shared_ptr<desktop::app::translation_service> m_translation_service;
		std::shared_ptr<desktop::updates::update_service> m_update_service;
		desktop::updates::version m_latest_version;
	};
}