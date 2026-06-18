#include "views/preferences_dialog.h"

using namespace application::controllers;
using namespace application::linux::helpers;
using namespace desktop::app;

namespace application::linux::views
{
	preferences_dialog::preferences_dialog(std::shared_ptr<preferences_view_controller> controller,
	                                       const std::shared_ptr<translation_service>& translation_service)
	    : m_controller{ std::move(controller) },
	      m_builder{ "preferences_dialog", translation_service },
	      m_dialog{ m_builder.get<AdwDialog>("root") }
	{
	}

	AdwDialog* preferences_dialog::gobj() const
	{
		return m_dialog;
	}

	void preferences_dialog::present(GtkWidget* parent)
	{
		adw_dialog_present(m_dialog, parent);
	}
}