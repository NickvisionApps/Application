#include "InfoBar.h"

namespace NickvisionApplication::Controls
{
	InfoBar::InfoBar(wxWindow* parent, long id, bool isLightTheme) : wxInfoBar(parent, id), m_isLightTheme(isLightTheme)
	{
		AddButton(wxID_CLOSE, "OK");
	}

	void InfoBar::ShowMessage(const wxString& message, int icon)
	{
		Hide();
		if (icon == wxICON_INFORMATION)
		{
			if (m_isLightTheme)
			{
				SetBackgroundColour({ 223, 246, 221 });
			}
			else
			{
				SetBackgroundColour({ 57, 61, 27 });
				SetForegroundColour(*wxWHITE);
			}
		}
		else if (icon == wxICON_WARNING)
		{
			if (m_isLightTheme)
			{
				SetBackgroundColour({ 255, 244, 206 });
			}
			else
			{
				SetBackgroundColour({ 67, 53, 25 });
				SetForegroundColour(*wxWHITE);
			}
		}
		else if (icon == wxICON_ERROR)
		{
			if (m_isLightTheme)
			{
				SetBackgroundColour({ 253, 231, 233 });
			}
			else
			{
				SetBackgroundColour({ 68, 39, 38 });
				SetForegroundColour(*wxWHITE);
			}
		}
		else
		{
			if (m_isLightTheme)
			{
				SetBackgroundColour({ 251, 251, 251 });
			}
			else
			{
				SetBackgroundColour({ 43, 43, 43 });
				SetForegroundColour(*wxWHITE);
			}
		}
		wxInfoBar::ShowMessage(message, icon);
	}
}
