#include "SettingsDialog.h"
#include <dwmapi.h>

namespace NickvisionApplication::Views
{
	SettingsDialog::SettingsDialog(wxWindow* parent, bool isLightTheme) : wxDialog(parent, IDs::DIALOG, "Settings", wxDefaultPosition, { 500, 500 })
	{
		//==Window Settings==//
		SetBackgroundColour(isLightTheme ? wxColour(243, 243, 243) : wxColour(32, 32, 32));
		CenterOnParent();
		//==Prefer Light Theme==//
		m_chkPreferLightTheme = new wxCheckBox(this, IDs::CHK_PREFER_LIGHT_THEME, "Prefer Light Theme");
		m_chkPreferLightTheme->SetToolTip(new wxToolTip("If checked, Application will display in a light theme. If not checked, a dark theme."));
		m_chkPreferLightTheme->SetValue(m_configuration.PreferLightTheme());
		//==Restart Required==//
		m_lblRestartRequired = new wxStaticText(this, IDs::LBL_RESTART_REQUIRED, "An application restart is required to apply a theme change.");
		m_lblRestartRequired->SetForegroundColour(*wxRED);
		//==Layout==//
		m_mainBox = new wxBoxSizer(wxVERTICAL);
		m_mainBox->Add(m_chkPreferLightTheme, 0, wxLEFT | wxTOP, 6);
		m_mainBox->Add(m_lblRestartRequired, 0, wxLEFT | wxTOP, 6);
		SetSizer(m_mainBox);
		//==Theme==//
		if (isLightTheme) //Light
		{
			
		}
		else //Dark
		{
			//Dark Title Bar
			BOOL enabled = TRUE;
			DwmSetWindowAttribute(GetHWND(), DWMWA_USE_IMMERSIVE_DARK_MODE, &enabled, sizeof(enabled));
			//Prefer Light Theme
			m_chkPreferLightTheme->SetForegroundColour(*wxWHITE);
		}
	}

	SettingsDialog::~SettingsDialog()
	{
		m_configuration.SetPreferLightTheme(m_chkPreferLightTheme->GetValue());
		m_configuration.Save();
	}
}