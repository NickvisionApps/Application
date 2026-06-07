#include "views/main_window.h"

using namespace application::controllers;
using namespace desktop::app;

namespace application::linux::views
{
	main_window::main_window(std::shared_ptr<main_window_controller> controller, std::shared_ptr<translation_service> translation_service)
	    : m_controller{ std::move(controller) },
	      m_translation_service{ std::move(translation_service) },
	      m_builder{ "main_window", m_translation_service },
	      m_window{ m_builder.get<AdwApplicationWindow>("root") }
	{
	}

	main_window::~main_window()
	{
		gtk_window_destroy(GTK_WINDOW(m_window));
	}

	AdwApplicationWindow* main_window::gobj() const
	{
		return m_window;
	}

	void main_window::present()
	{
		gtk_window_present(GTK_WINDOW(m_window));
	}
}