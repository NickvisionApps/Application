#include "helpers/gtk.h"
#include <array>
#include <format>

namespace application::linux::helpers
{
	void gtk::add_action(GActionMap* map, const char* action_name, std::function<void()> callback, const char* shortcut)
	{
		GSimpleAction* action{ g_simple_action_new(action_name, nullptr) };
		g_action_map_add_action(map, G_ACTION(action));
		g_signal_connect_data(action, "activate",
		                      G_CALLBACK(+[](GSimpleAction*, GVariant*, gpointer data)
		{
			std::function<void()>* callback{ static_cast<std::function<void()>*>(data) };
			(*callback)();
		}),
		                      new std::function<void()>(std::move(callback)),
		                      +[](gpointer data, GClosure*)
		{
			std::function<void()>* callback{ static_cast<std::function<void()>*>(data) };
			delete callback;
		},
		                      G_CONNECT_DEFAULT);
		if (shortcut)
		{
			std::array<const char*, 2> shortcuts{ shortcut, nullptr };
			gtk_application_set_accels_for_action(GTK_APPLICATION(g_application_get_default()), std::format("win.{}", action_name).c_str(), shortcuts.data());
		}
	}

	void gtk::add_action(AdwApplicationWindow* window, const char* action_name, std::function<void()> callback, const char* shortcut)
	{
		add_action(G_ACTION_MAP(window), action_name, std::move(callback), shortcut);
	}

	void gtk::invoke_on_main_thread(std::function<void()> function)
	{
		g_idle_add(+[](gpointer data) -> gboolean
		{
			std::function<void()>* function{ static_cast<std::function<void()>*>(data) };
			(*function)();
			delete function;
			return G_SOURCE_REMOVE;
		}, new std::function<void()>(std::move(function)));
	}
}