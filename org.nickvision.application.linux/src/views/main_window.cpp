#include "views/main_window.h"
#include <format>
#include <string>
#include "helpers/gtk.h"

using namespace application::controllers;
using namespace application::events;
using namespace application::linux::helpers::gtk;
using namespace application::services;
using namespace desktop::app;
using namespace desktop::hosting;
using namespace desktop::notifications;

namespace application::linux::views
{
	main_window::main_window(std::shared_ptr<main_window_controller> controller, std::shared_ptr<app_info> app_info,
	                         const std::shared_ptr<events_service>& events_service, std::shared_ptr<lifetime_service> lifetime_service,
	                         std::shared_ptr<translation_service> translation_service)
	    : m_controller{ std::move(controller) },
	      m_app_info{ std::move(app_info) },
	      m_lifetime_service{ std::move(lifetime_service) },
	      m_translation_service{ std::move(translation_service) },
	      m_builder{ "main_window", m_translation_service },
	      m_window{ m_builder.get<AdwApplicationWindow>("root") }
	{
		events_service->get_app_notification_sent_event() += [this](const notification_service&, const app_notification_sent_event_args& args)
		{
			invoke_on_main_thread([this, args]()
			{
				on_app_notification_sent(args);
			});
		};
		events_service->get_folder_changed_event() += [this](const folder_service&, const folder_changed_event_args& args)
		{
			on_folder_changed(args);
		};
		g_signal_connect(m_window, "close-request",
		                 G_CALLBACK(+[](GtkWindow*, gpointer data) -> bool
		{
			return static_cast<main_window*>(data)->on_close_request();
		}),
		                 this);
		add_action(m_window, "about", [this]()
		{
			about();
		}, "F1");
		add_action(m_window, "close_folder", [this]()
		{
			close_folder();
		}, "<Primary>W");
		add_action(m_window, "open_folder", [this]()
		{
			open_folder();
		}, "<Primary>O");
		add_action(m_window, "preferences", [this]()
		{
			preferences();
		}, "<Primary>comma");
		add_action(m_window, "quit", [this]()
		{
			on_close_request();
		}, "<Primary>Q");
		add_action(m_window, "shortcuts", [this]()
		{
			shortcuts();
		}, "<Primary>question");
		gtk_window_set_title(GTK_WINDOW(m_window), m_app_info->get_short_name().c_str());
		gtk_window_set_icon_name(GTK_WINDOW(m_window), m_app_info->get_id().c_str());
		if (m_app_info->get_version().is_preview())
		{
			gtk_widget_add_css_class(GTK_WIDGET(m_window), "devel");
		}
		adw_status_page_set_title(m_builder.get<AdwStatusPage>("home_page"), m_controller->get_greeting().c_str());
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

	void main_window::about()
	{
		std::string extra;
		extra += std::format("gtk {}.{}.{}\n", gtk_get_major_version(), gtk_get_minor_version(), gtk_get_micro_version());
		extra += std::format("libadwaita {}.{}.{}\n", adw_get_major_version(), adw_get_minor_version(), adw_get_micro_version());
		AdwAboutDialog* dialog{ ADW_ABOUT_DIALOG(adw_about_dialog_new()) };
		adw_dialog_present(ADW_DIALOG(dialog), GTK_WIDGET(m_window));
	}

	void main_window::close_folder()
	{
		m_controller->close_folder();
	}

	void main_window::on_app_notification_sent(const app_notification_sent_event_args& args)
	{
		AdwToast* toast{ adw_toast_new(args.get_notification().get_message().c_str()) };
		if (args.get_notification().get_action() == "close")
		{
			adw_toast_set_button_label(toast, m_translation_service->_("Close"));
			g_signal_connect(toast, "button-clicked",
			                 G_CALLBACK(+[](AdwToast*, gpointer data)
			{
				static_cast<main_window*>(data)->close_folder();
			}),
			                 this);
		}
		adw_toast_overlay_add_toast(m_builder.get<AdwToastOverlay>("toast_overlay"), toast);
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

	void main_window::on_folder_changed(const folder_changed_event_args& args)
	{
	}

	void main_window::open_folder()
	{
		GtkFileDialog* dialog{ gtk_file_dialog_new() };
		gtk_file_dialog_set_title(dialog, m_translation_service->_("Open Folder"));
		gtk_file_dialog_select_folder(dialog, GTK_WINDOW(m_window), nullptr,
		                              +[](GObject* source, GAsyncResult* res, gpointer data)
		{
			main_window* self{ static_cast<main_window*>(data) };
			GError* error{ nullptr };
			GFile* folder{ gtk_file_dialog_select_folder_finish(GTK_FILE_DIALOG(source), res, &error) };
			if (error)
			{
				g_error_free(error);
				return;
			}
			if (folder)
			{
				self->m_controller->open_folder(g_file_get_path(folder));
				g_object_unref(folder);
			}
		},
		                              this);
	}

	void main_window::preferences()
	{
	}

	void main_window::shortcuts()
	{
	}
}