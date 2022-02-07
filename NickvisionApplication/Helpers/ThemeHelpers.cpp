#include "ThemeHelpers.h"
#include <dwmapi.h>

namespace NickvisionApplication::Helpers
{
	wxColor ThemeHelpers::GetMainDarkColor()
	{
		return { 32, 32, 32 };
	}

	wxColor ThemeHelpers::GetSecondaryDarkColor()
	{
		return { 43, 43, 43 };
	}

	wxColor ThemeHelpers::GetMainLightColor()
	{
		return { 243, 243, 243 };
	}

	wxColor ThemeHelpers::GetSecondaryLightColor()
	{
		return { 251, 251, 251 };
	}

	void ThemeHelpers::ApplyWin32DarkMode(wxWindow* window)
	{
		BOOL enabled = TRUE;
		DwmSetWindowAttribute(window->GetHWND(), DWMWA_USE_IMMERSIVE_DARK_MODE, &enabled, sizeof(enabled));
		SetWindowTheme(window->GetHWND(), L"Explorer", NULL);
	}
}