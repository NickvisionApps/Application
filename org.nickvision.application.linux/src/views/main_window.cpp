#include "views/main_window.h"
#include <format>
#include <string>
#include <vector>
#include "controls/shortcuts_dialog.h"
#include "helpers/gtk.h"
#include "views/preferences_dialog.h"

using namespace application::controllers;
using namespace application::events;
using namespace application::linux::controls;
using namespace application::linux::helpers;
using namespace application::services;
using namespace desktop::app;
using namespace desktop::hosting;
using namespace desktop::notifications;
using namespace desktop::services;

namespace application::linux::views
{
	static std::vector<const char*> to_char_list(const std::vector<std::string>& list)
	{
		std::vector<const char*> res;
		res.reserve(list.size() + 1);
		for (const std::string& s : list)
		{
			res.push_back(s.c_str());
		}
		res.push_back(nullptr);
		return res;
	}

	main_window::main_window(std::shared_ptr<main_window_controller> controller, std::shared_ptr<service_provider> service_provider,
	                         std::shared_ptr<app_info> app_info, const std::shared_ptr<events_service>& events_service,
	                         std::shared_ptr<lifetime_service> lifetime_service, std::shared_ptr<translation_service> translation_service)
	    : m_controller{ std::move(controller) },
	      m_service_provider{ std::move(service_provider) },
	      m_app_info{ std::move(app_info) },
	      m_lifetime_service{ std::move(lifetime_service) },
	      m_translation_service{ std::move(translation_service) },
	      m_builder{ "main_window", m_translation_service },
	      m_window{ m_builder.get<AdwApplicationWindow>("root") }
	{
		events_service->get_app_notification_sent_event() += [this](const notification_service& /* unused */, const app_notification_sent_event_args& args)
		{
			gtk::invoke_on_main_thread([this, args]()
			{
				on_app_notification_sent(args);
			});
		};
		events_service->get_folder_changed_event() += [this](const folder_service& /* unused */, const folder_changed_event_args& args)
		{
			on_folder_changed(args);
		};
		g_signal_connect(m_window, "close-request",
		                 G_CALLBACK(+[](GtkWindow* /* unused */, gpointer data) -> bool
		{
			return static_cast<main_window*>(data)->on_close_request();
		}),
		                 this);
		gtk::add_action(m_window, "about", [this]()
		{
			about();
		}, "F1");
		gtk::add_action(m_window, "close_folder", [this]()
		{
			close_folder();
		}, "<Primary>W");
		gtk::add_action(m_window, "open_folder", [this]()
		{
			open_folder();
		}, "<Primary>O");
		gtk::add_action(m_window, "preferences", [this]()
		{
			preferences();
		}, "<Primary>comma");
		gtk::add_action(m_window, "quit", [this]()
		{
			on_close_request();
		}, "<Primary>Q");
		gtk::add_action(m_window, "shortcuts", [this]()
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
		std::string extra{ std::format("gtk {}.{}.{}\n", gtk_get_major_version(), gtk_get_minor_version(), gtk_get_micro_version()) };
		extra += std::format("libadwaita {}.{}.{}", adw_get_major_version(), adw_get_minor_version(), adw_get_micro_version());
		AdwAboutDialog* dialog{ ADW_ABOUT_DIALOG(adw_about_dialog_new()) };
		adw_about_dialog_set_application_name(dialog, m_app_info->get_short_name().c_str());
		adw_about_dialog_set_application_icon(dialog, m_app_info->get_version().is_preview() ? std::string(m_app_info->get_id() + "-devel").c_str()
		                                                                                     : m_app_info->get_id().c_str());
		adw_about_dialog_set_developer_name(dialog, "Nickvision");
		adw_about_dialog_set_version(dialog, m_app_info->get_version().str().c_str());
		adw_about_dialog_set_release_notes(dialog, m_app_info->get_changelog_html().c_str());
		adw_about_dialog_set_debug_info(dialog, m_controller->get_debugging_information(extra).c_str());
		adw_about_dialog_set_comments(dialog, m_app_info->get_description().c_str());
		adw_about_dialog_set_license_type(dialog, GTK_LICENSE_MIT_X11);
		adw_about_dialog_set_copyright(dialog, "© Nickvision 2021-2026");
		adw_about_dialog_set_website(dialog, "https://nickvision.org");
		adw_about_dialog_set_issue_url(dialog, m_app_info->get_issues_url().c_str());
		adw_about_dialog_set_support_url(dialog, m_app_info->get_discussions_url().c_str());
		adw_about_dialog_add_link(dialog, m_translation_service->_("GitHub Repo"), m_app_info->get_source_url().c_str());
		for (const std::pair<const std::string, std::string>& link : m_app_info->get_extra_links())
		{
			adw_about_dialog_add_link(dialog, link.first.c_str(), link.second.c_str());
		}
		std::vector<std::string> artists;
		artists.reserve(m_app_info->get_artists().size());
		for (const std::pair<const std::string, std::string>& artist : m_app_info->get_artists())
		{
			artists.push_back(std::format("{} {}", artist.first, artist.second));
		}
		std::vector<std::string> designers;
		designers.reserve(m_app_info->get_designers().size());
		for (const std::pair<const std::string, std::string>& designer : m_app_info->get_designers())
		{
			designers.push_back(std::format("{} {}", designer.first, designer.second));
		}
		std::vector<std::string> developers;
		developers.reserve(m_app_info->get_developers().size());
		for (const std::pair<const std::string, std::string>& developer : m_app_info->get_developers())
		{
			developers.push_back(std::format("{} {}", developer.first, developer.second));
		}
		adw_about_dialog_set_artists(dialog, to_char_list(artists).data());
		adw_about_dialog_set_designers(dialog, to_char_list(designers).data());
		adw_about_dialog_set_developers(dialog, to_char_list(developers).data());
		if (!m_app_info->get_translation_credits().empty() && m_app_info->get_translation_credits() != "translation-credits")
		{
			adw_about_dialog_set_translator_credits(dialog, m_app_info->get_translation_credits().c_str());
		}
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
			                 G_CALLBACK(+[](AdwToast* /* unused */, gpointer data)
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
		m_controller->set_window_geometry({ height, width, static_cast<bool>(gtk_window_is_maximized(GTK_WINDOW(m_window))) });
		gtk_window_destroy(GTK_WINDOW(m_window));
		m_lifetime_service->request_stop();
		return false;
	}

	void main_window::on_folder_changed(const folder_changed_event_args& args)
	{
		adw_view_stack_set_visible_child_name(m_builder.get<AdwViewStack>("stack"), args.get_path().empty() ? "home_page" : "folder_page");
		adw_status_page_set_title(m_builder.get<AdwStatusPage>("folder_page"), args.get_path().filename().c_str());
		adw_status_page_set_description(m_builder.get<AdwStatusPage>("folder_page"),
		                                m_translation_service->_n("{} file", "{} files", args.get_files().size(), args.get_files().size()).c_str());
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
		std::shared_ptr<preferences_dialog> dialog{ m_service_provider->get_required<preferences_dialog>() };
		dialog->present(GTK_WIDGET(m_window));
	}

	void main_window::shortcuts()
	{
		std::shared_ptr<shortcuts_dialog> dialog{ m_service_provider->get_required<shortcuts_dialog>() };
		dialog->present(GTK_WIDGET(m_window));
	}
}