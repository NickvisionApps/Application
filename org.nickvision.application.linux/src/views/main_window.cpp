#include "views/main_window.h"

using namespace application::controllers;
using namespace desktop::app;
using namespace desktop::hosting;

namespace application::linux::views
{
	main_window::main_window(std::shared_ptr<main_window_controller> controller, std::shared_ptr<app_info> app_info,
	                         std::shared_ptr<lifetime_service> lifetime_service, std::shared_ptr<translation_service> translation_service)
	    : m_controller{ std::move(controller) },
	      m_app_info{ std::move(app_info) },
	      m_lifetime_service{ std::move(lifetime_service) },
	      m_translation_service{ std::move(translation_service) },
	      m_builder{ "main_window", m_translation_service },
	      m_window{ m_builder.get<AdwApplicationWindow>("root") }
	{
		gtk_window_set_title(GTK_WINDOW(m_window), m_app_info->get_short_name().c_str());
		gtk_window_set_icon_name(GTK_WINDOW(m_window), m_app_info->get_id().c_str());
		if (m_app_info->get_version().is_preview())
		{
			gtk_widget_add_css_class(GTK_WIDGET(m_window), "devel");
		}
		adw_status_page_set_title(m_builder.get<AdwStatusPage>("home_page"), m_controller->get_greeting().c_str());
		g_signal_connect(m_window, "close-request",
		                 G_CALLBACK(+[](GtkWindow* window, gpointer data) -> bool
		{
			return static_cast<main_window*>(data)->on_close_request();
		}),
		                 this);
		window_geometry geometry{ m_controller->get_window_geometry() };
		gtk_window_set_default_size(GTK_WINDOW(m_window), geometry.get_width(), geometry.get_height());
		if (geometry.is_maximized())
		{
			gtk_window_maximize(GTK_WINDOW(m_window));
		}
		m_controller->queue_check_for_updates(false);
	}

	AdwApplicationWindow* main_window::gobj() const
	{
		return m_window;
	}

	void main_window::present()
	{
		gtk_window_present(GTK_WINDOW(m_window));
	}

	bool main_window::on_close_request()
	{
		if (!m_controller->can_shutdown())
		{
			return true;
		}
		int width{ 0 };
		int height{ 0 };
		gtk_window_get_default_size(GTK_WINDOW(m_window), &width, &height);
		m_controller->set_window_geometry({ width, height, static_cast<bool>(gtk_window_is_maximized(GTK_WINDOW(m_window))) });
		gtk_window_destroy(GTK_WINDOW(m_window));
		m_lifetime_service->request_stop();
		return false;
	}
}