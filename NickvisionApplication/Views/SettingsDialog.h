#pragma once

#include <wx/wx.h>
#include "../Models/Configuration.h"

namespace NickvisionApplication::Views
{
	class SettingsDialog : public wxDialog
	{
	public:
		SettingsDialog(wxWindow* parent, bool isLightTheme);
		~SettingsDialog();

	private:
		enum IDs
		{
			DIALOG = 300,
			CHK_PREFER_LIGHT_THEME,
			LBL_RESTART_REQUIRED
		};
		NickvisionApplication::Models::Configuration m_configuration;
		wxBoxSizer* m_mainBox = nullptr;
		wxCheckBox* m_chkPreferLightTheme = nullptr;
		wxStaticText* m_lblRestartRequired = nullptr;
	};
}

