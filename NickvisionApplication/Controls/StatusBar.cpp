#include "StatusBar.h"
#include "../Helpers/ThemeHelpers.h"

namespace NickvisionApplication::Controls
{
	using namespace NickvisionApplication::Helpers;

	StatusBar::StatusBar(wxWindow* parent, long id, bool isLightTheme) : wxStatusBar(parent, id, wxSTB_SIZEGRIP)
	{
		m_lblMessage = new wxStaticText(this, wxID_ANY, _("Ready"), wxPoint(6, 3), wxDefaultSize);
		if (isLightTheme)
		{
			SetBackgroundColour(ThemeHelpers::GetSecondaryLightColor());
		}
		else
		{
			SetBackgroundColour(ThemeHelpers::GetSecondaryDarkColor());
			m_lblMessage->SetForegroundColour(*wxWHITE);
		}
	}

	const wxString& StatusBar::GetMessage() const
	{
		return m_lblMessage->GetLabelText();
	}

	void StatusBar::SetMessage(const wxString& message)
	{
		m_lblMessage->SetLabelText(message);
	}
}