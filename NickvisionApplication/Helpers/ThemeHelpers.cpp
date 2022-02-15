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

	wxColor ThemeHelpers::GetTertiaryDarkColor()
	{
		return { 60, 60, 60 };
	}

	wxColor ThemeHelpers::GetMainLightColor()
	{
		return { 243, 243, 243 };
	}

	wxColor ThemeHelpers::GetSecondaryLightColor()
	{
		return { 251, 251, 251 };
	}

	wxColor ThemeHelpers::GetTertiaryLightColor()
	{
		return { 250, 250, 250 };
	}

	void ThemeHelpers::ApplyWin32DarkMode(wxWindow* window)
	{
		BOOL enabled = TRUE;
		DwmSetWindowAttribute(window->GetHWND(), DWMWA_USE_IMMERSIVE_DARK_MODE, &enabled, sizeof(enabled));
		SetWindowTheme(window->GetHWND(), L"DarkMode_Explorer", NULL);
	}

	void ThemeHelpers::ApplyWin32LightMode(wxWindow* window)
	{
		BOOL enabled = FALSE;
		DwmSetWindowAttribute(window->GetHWND(), DWMWA_USE_IMMERSIVE_DARK_MODE, &enabled, sizeof(enabled));
		SetWindowTheme(window->GetHWND(), L"Explorer", NULL);
	}
}