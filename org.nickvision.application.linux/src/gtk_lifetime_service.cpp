#include "gtk_lifetime_service.h"
#include <filesystem>
#include "services/app_config_service.h"
#include "views/main_window.h"

using namespace application::linux::views;
using namespace application::models;
using namespace application::services;
using namespace desktop::app;
using namespace desktop::hosting;
using namespace desktop::services;
using namespace desktop::system;

namespace application::linux
{
	gtk_lifetime_service::gtk_lifetime_service(std::shared_ptr<service_provider> service_provider)
	    : lifetime_service{ service_provider->get_required<app_info>() },
	      m_service_provider{ std::move(service_provider) }
	{
	}

	gtk_lifetime_service::~gtk_lifetime_service()
	{
		if (m_application)
		{
			g_object_unref(m_application);
		}
	}

	void gtk_lifetime_service::on_startup_and_run()
	{
		std::shared_ptr<app_info> info{ m_service_provider->get_required<app_info>() };
		std::shared_ptr<arguments_service> arguments{ m_service_provider->get_required<arguments_service>() };
		m_application = adw_application_new(info->get_id().c_str(), G_APPLICATION_DEFAULT_FLAGS);
		GError* error{ nullptr };
		GResource* resource{ g_resource_load((environment::get_executable_directory() / (info->get_id() + ".gresource")).string().c_str(), &error) };
		if (error)
		{
			return;
		}
		g_resources_register(resource);
		g_signal_connect(m_application, "startup",
		                 G_CALLBACK(+[](GtkApplication* app, gpointer data)
		{
			gtk_lifetime_service* self{ static_cast<gtk_lifetime_service*>(data) };
			std::shared_ptr<app_config_service> config{ self->m_service_provider->get_required<app_config_service>() };
			switch (config->get_theme())
			{
			case theme::light:
				adw_style_manager_set_color_scheme(adw_application_get_style_manager(ADW_APPLICATION(app)), ADW_COLOR_SCHEME_FORCE_LIGHT);
				break;
			case theme::dark:
				adw_style_manager_set_color_scheme(adw_application_get_style_manager(ADW_APPLICATION(app)), ADW_COLOR_SCHEME_FORCE_DARK);
				break;
			case theme::system:
				adw_style_manager_set_color_scheme(adw_application_get_style_manager(ADW_APPLICATION(app)), ADW_COLOR_SCHEME_DEFAULT);
				break;
			}
		}),
		                 this);
		g_signal_connect(m_application, "activate",
		                 G_CALLBACK(+[](GtkApplication* app, gpointer data)
		{
			gtk_lifetime_service* self{ static_cast<gtk_lifetime_service*>(data) };
			std::shared_ptr<main_window> window{ self->m_service_provider->get_required<main_window>() };
			gtk_application_add_window(app, GTK_WINDOW(window->gobj()));
			window->present();
		}),
		                 this);
		if (g_application_run(G_APPLICATION(m_application), static_cast<int>(arguments->get_count()), arguments->argv()) != 0)
		{
			throw std::runtime_error("Error running application");
		}
	}

	void gtk_lifetime_service::on_shutdown() noexcept
	{
		g_application_quit(G_APPLICATION(m_application));
		g_object_unref(m_application);
		m_application = nullptr;
	}

	void gtk_lifetime_service::on_stop_requested() noexcept
	{
		g_application_quit(G_APPLICATION(m_application));
	}
}