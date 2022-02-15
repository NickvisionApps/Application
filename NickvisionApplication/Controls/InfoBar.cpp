#include "InfoBar.h"
#include "../Helpers/ThemeHelpers.h"

namespace NickvisionApplication::Controls
{
	using namespace NickvisionApplication::Helpers;

	InfoBar::InfoBar(wxWindow* parent, long id) : wxInfoBar(parent, id)
	{
		AddButton(wxID_CLOSE, "OK");
		SetIsLightTheme(false);
	}

	void InfoBar::SetIsLightTheme(bool isLightTheme)
	{
		m_isLightTheme = isLightTheme;
		if (m_isLightTheme)
		{
			SetForegroundColour(*wxBLACK);
		}
		else
		{
			SetForegroundColour(*wxWHITE);
		}
		Refresh();
	}

	void InfoBar::ShowMessage(const wxString& message, int icon)
	{
		Hide();
		if (icon == wxICON_INFORMATION)
		{
			SetBackgroundColour(m_isLightTheme ? wxColor(223, 246, 221) : wxColor(57, 61, 27));
		}
		else if (icon == wxICON_WARNING)
		{
			SetBackgroundColour(m_isLightTheme ? wxColor(255, 244, 206) : wxColor(67, 53, 25));
		}
		else if (icon == wxICON_ERROR)
		{
			SetBackgroundColour(m_isLightTheme ? wxColor(253, 231, 233) : wxColor(68, 39, 38));
		}
		else
		{
			SetBackgroundColour(m_isLightTheme ? ThemeHelpers::GetTertiaryLightColor() : ThemeHelpers::GetTertiaryDarkColor());
		}
		wxInfoBar::ShowMessage(message, icon);
	}
}
