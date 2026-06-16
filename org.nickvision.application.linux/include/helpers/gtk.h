#pragma once

#include <adwaita.h>
#include <functional>
#include <libdesktop.h>

#undef linux

namespace application::linux::helpers::gtk
{
	void add_action(GActionMap* map, const char* action_name, std::function<void()> callback, const char* shortcut = nullptr);
	void add_action(AdwApplicationWindow* window, const char* action_name, std::function<void()> callback, const char* shortcut = nullptr);
	void invoke_on_main_thread(std::function<void()> function);
}