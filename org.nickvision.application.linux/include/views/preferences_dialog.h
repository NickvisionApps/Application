#pragma once

#include <adwaita.h>
#include <libdesktop.h>
#include <memory>
#include <tuple>
#include "controllers/preferences_view_controller.h"
#include "helpers/ui_builder.h"

#undef linux

namespace application::linux::views
{
	class preferences_dialog
	{
	public:
		using dependencies = std::tuple<controllers::preferences_view_controller, desktop::app::translation_service>;
		preferences_dialog(std::shared_ptr<controllers::preferences_view_controller> controller,
		                   const std::shared_ptr<desktop::app::translation_service>& translation_service);
		~preferences_dialog() = default;
		preferences_dialog(const preferences_dialog&) = delete;
		preferences_dialog(preferences_dialog&&) = delete;
		AdwDialog* gobj() const;
		void present(GtkWidget* parent);
		preferences_dialog& operator=(const preferences_dialog&) = delete;
		preferences_dialog& operator=(preferences_dialog&&) = delete;

	private:
		std::shared_ptr<controllers::preferences_view_controller> m_controller;
		helpers::ui_builder m_builder;
		AdwDialog* m_dialog{ nullptr };
	};
}