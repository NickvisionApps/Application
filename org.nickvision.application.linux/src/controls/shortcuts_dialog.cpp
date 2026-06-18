#include "controls/shortcuts_dialog.h"

using namespace application::linux::helpers;
using namespace desktop::app;

namespace application::linux::controls
{
	shortcuts_dialog::shortcuts_dialog(const std::shared_ptr<translation_service>& translation_service)
	    : m_builder{ "shortcuts_dialog", translation_service },
	      m_dialog{ m_builder.get<AdwShortcutsDialog>("root") }
	{
	}

	AdwShortcutsDialog* shortcuts_dialog::gobj() const
	{
		return m_dialog;
	}

	void shortcuts_dialog::present(GtkWidget* parent)
	{
		adw_dialog_present(ADW_DIALOG(m_dialog), parent);
	}
}