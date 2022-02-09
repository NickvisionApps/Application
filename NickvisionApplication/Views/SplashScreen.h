#pragma once

#include <thread>
#include <wx/wx.h>
#include "MainWindow.h"

namespace NickvisionApplication::Views
{
	class SplashScreen : public wxDialog
	{
	public:
		SplashScreen(MainWindow* parent);

	private:
		enum IDs
		{
			DIALOG = 500,
			PANEL_TITLE,
			LBL_TITLE,
			LBL_DESCRIPTION,
			PROG_BAR,
			LBL_PROGRESS,
			LBL_VERSION,
			LBL_COPYRIGHT
		};
		MainWindow* m_mainWindow = nullptr;
		std::jthread m_trdStartup;
		//==UI==//
		wxBoxSizer* m_mainBox = nullptr;
		wxPanel* m_panelTitle = nullptr;
		wxBoxSizer* m_boxTitle = nullptr;
		wxStaticText* m_lblTitle = nullptr;
		wxStaticText* m_lblDescription = nullptr;
		wxGauge* m_progBar = nullptr;
		wxStaticText* m_lblProgress = nullptr;
		wxBoxSizer* m_boxAppInfo = nullptr;
		wxStaticText* m_lblVersion = nullptr;
		wxStaticText* m_lblCopyright = nullptr;
	};
}

