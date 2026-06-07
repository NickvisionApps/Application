#pragma once

#include <adwaita.h>
#include <memory>
#include <tuple>
#include "controllers/main_window_controller.h"
#include "helpers/ui_builder.h"

#undef linux

namespace application::linux::views
{
	class main_window
	{
	public:
		using dependencies = std::tuple<controllers::main_window_controller, desktop::app::translation_service>;
		main_window(std::shared_ptr<controllers::main_window_controller> controller, std::shared_ptr<desktop::app::translation_service> translation_service);
		~main_window();
		main_window(const main_window&) = delete;
		main_window(main_window&&) = delete;
		AdwApplicationWindow* gobj() const;
		void present();
		main_window& operator=(const main_window&) = delete;
		main_window& operator=(main_window&&) = delete;

	private:
		std::shared_ptr<controllers::main_window_controller> m_controller;
		std::shared_ptr<desktop::app::translation_service> m_translation_service;
		helpers::ui_builder m_builder;
		AdwApplicationWindow* m_window{ nullptr };
	};
}