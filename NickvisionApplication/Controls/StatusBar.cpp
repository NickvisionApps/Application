#include "StatusBar.h"

namespace NickvisionApplication::Controls
{
	StatusBar::StatusBar(wxWindow* parent, long id, bool isLightTheme) : wxStatusBar(parent, id, wxSTB_SIZEGRIP)
	{
		m_lblMessage = new wxStaticText(this, wxID_ANY, _("Ready"), wxPoint(6, 3), wxDefaultSize);
		if (isLightTheme)
		{
			SetBackgroundColour({ 251, 251, 251 });
		}
		else
		{
			SetBackgroundColour({ 43, 43, 43 });
			m_lblMessage->SetForegroundColour(*wxWHITE);
		}
	}

	wxString StatusBar::GetMessage() const
	{
		return m_lblMessage->GetLabelText();
	}

	void StatusBar::SetMessage(const wxString& message)
	{
		m_lblMessage->SetLabelText(message);
	}
}