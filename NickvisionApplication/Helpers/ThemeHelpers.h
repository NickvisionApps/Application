#pragma once

#include <wx/wx.h>

namespace NickvisionApplication::Helpers::ThemeHelpers
{
	wxColor GetMainDarkColor();
	wxColor GetSecondaryDarkColor();
	wxColor GetTertiaryDarkColor();
	wxColor GetMainLightColor();
	wxColor GetSecondaryLightColor();
	wxColor GetTertiaryLightColor();
	void ApplyWin32DarkMode(wxWindow* window);
	void ApplyWin32LightMode(wxWindow* window);
}

