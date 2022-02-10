#pragma once

#include <wx/wx.h>

namespace NickvisionApplication::Controls
{
	class StatusBar : public wxStatusBar
	{
	public:
		StatusBar(wxWindow* parent, long id, bool isLightTheme);
		const wxString& GetMessage() const;
		void SetMessage(const wxString& message);

	private:
		wxStaticText* m_lblMessage;
	};
}

