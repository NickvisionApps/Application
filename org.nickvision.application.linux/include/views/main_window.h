#pragma once

#include <adwaita.h>
#include <libdesktop.h>
#include <memory>
#include <tuple>
#include "controllers/main_window_controller.h"
#include "helpers/ui_builder.h"
#include "services/events_service.h"

#undef linux

namespace application::linux::views
{
	class main_window
	{
	public:
		using dependencies = std::tuple<controllers::main_window_controller, desktop::services::service_provider, desktop::app::app_info,
		                                services::events_service, desktop::hosting::lifetime_service, desktop::app::translation_service>;
		main_window(std::shared_ptr<controllers::main_window_controller> controller, std::shared_ptr<desktop::services::service_provider> service_provider,
		            std::shared_ptr<desktop::app::app_info> app_info, const std::shared_ptr<services::events_service>& events_service,
		            std::shared_ptr<desktop::hosting::lifetime_service> lifetime_service,
		            std::shared_ptr<desktop::app::translation_service> translation_service);
		~main_window() = default;
		main_window(const main_window&) = delete;
		main_window(main_window&&) = delete;
		AdwApplicationWindow* gobj() const;
		void present();
		main_window& operator=(const main_window&) = delete;
		main_window& operator=(main_window&&) = delete;

	private:
		void about();
		void close_folder();
		void on_app_notification_sent(const desktop::notifications::app_notification_sent_event_args& args);
		bool on_close_request();
		void on_folder_changed(const events::folder_changed_event_args& args);
		void open_folder();
		void preferences();
		void shortcuts();
		std::shared_ptr<controllers::main_window_controller> m_controller;
		std::shared_ptr<desktop::services::service_provider> m_service_provider;
		std::shared_ptr<desktop::app::app_info> m_app_info;
		std::shared_ptr<desktop::hosting::lifetime_service> m_lifetime_service;
		std::shared_ptr<desktop::app::translation_service> m_translation_service;
		helpers::ui_builder m_builder;
		AdwApplicationWindow* m_window{ nullptr };
	};
}