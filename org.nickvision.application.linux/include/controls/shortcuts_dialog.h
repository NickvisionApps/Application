#pragma once

#include <adwaita.h>
#include <libdesktop.h>
#include <memory>
#include <tuple>
#include "helpers/ui_builder.h"

#undef linux

namespace application::linux::controls
{
	class shortcuts_dialog
	{
	public:
		using dependencies = std::tuple<desktop::app::translation_service>;
		shortcuts_dialog(const std::shared_ptr<desktop::app::translation_service>& translation_service);
		~shortcuts_dialog() = default;
		shortcuts_dialog(const shortcuts_dialog&) = delete;
		shortcuts_dialog(shortcuts_dialog&&) = delete;
		AdwShortcutsDialog* gobj() const;
		void present(GtkWidget* parent);
		shortcuts_dialog& operator=(const shortcuts_dialog&) = delete;
		shortcuts_dialog& operator=(shortcuts_dialog&&) = delete;

	private:
		helpers::ui_builder m_builder;
		AdwShortcutsDialog* m_dialog{ nullptr };
	};
}