#include "SplashScreen.h"
#include <chrono>
#include "../Models/Configuration.h"
#include "../Helpers/ThemeHelpers.h"

namespace NickvisionApplication::Views
{
	using namespace NickvisionApplication::Models;
	using namespace NickvisionApplication::Helpers;

	SplashScreen::SplashScreen(MainWindow* parent) : wxDialog(parent, IDs::DIALOG, "", wxDefaultPosition, { 700, 400 }, 0), m_mainWindow(parent)
	{
		//==Window Settings==//
		CenterOnScreen();
		//==Title Panel==//
		//Panel
		m_panelTitle = new wxPanel(this, IDs::PANEL_TITLE, wxDefaultPosition, { -1, 160 });
		m_panelTitle->SetBackgroundColour({ 0, 71, 171 });
		//Title
		m_lblTitle = new wxStaticText(m_panelTitle, IDs::LBL_TITLE, "NickvisionApplication");
		m_lblTitle->SetForegroundColour(*wxWHITE);
		wxFont fontTitle = m_lblTitle->GetFont();
		fontTitle.SetPointSize(20);
		m_lblTitle->SetFont(fontTitle);
		//Description
		m_lblDescription = new wxStaticText(m_panelTitle, IDs::LBL_DESCRIPTION, "A template for creating Nickvision applications.");
		m_lblDescription->SetForegroundColour(*wxWHITE);
		//Box
		m_boxTitle = new wxBoxSizer(wxVERTICAL);
		m_boxTitle->Add(m_lblTitle, 0, wxCENTER | wxTOP, 30);
		m_boxTitle->Add(m_lblDescription, 0, wxCENTER | wxTOP, 30);
		m_panelTitle->SetSizer(m_boxTitle);
		//==Progress==//
		//Prog Bar
		m_progBar = new wxGauge(this, IDs::PROG_BAR, 10);
		m_progBar->Pulse();
		//Prog Label
		m_lblProgress = new wxStaticText(this, IDs::LBL_PROGRESS, "Starting application...");
		//==App Info==//
		//Version
		m_lblVersion = new wxStaticText(this, IDs::LBL_VERSION, "Version: 2022.2.0");
		//Copyright
		m_lblCopyright = new wxStaticText(this, IDs::LBL_COPYRIGHT, "(C) Nickvision 2021-2022");
		//Box
		m_boxAppInfo = new wxBoxSizer(wxHORIZONTAL);
		m_boxAppInfo->Add(m_lblVersion, 0, wxLEFT, 6);
		m_boxAppInfo->AddStretchSpacer();
		m_boxAppInfo->Add(m_lblCopyright, 0, wxRIGHT, 6);
		//==Layout==//
		m_mainBox = new wxBoxSizer(wxVERTICAL);
		m_mainBox->Add(m_panelTitle, 0, wxEXPAND);
		m_mainBox->Add(m_progBar, 0, wxCENTER | wxEXPAND | wxLEFT | wxTOP | wxRIGHT, 40);
		m_mainBox->Add(m_lblProgress, 0, wxCENTER | wxTOP, 20);
		m_mainBox->AddStretchSpacer();
		m_mainBox->Add(m_boxAppInfo, 0, wxEXPAND | wxBOTTOM);
		SetSizer(m_mainBox);
		//==Theme==//
		Configuration configuration;
		if (configuration.PreferLightTheme())
		{
			SetBackgroundColour(ThemeHelpers::GetMainLightColor());
		}
		else
		{
			SetBackgroundColour(ThemeHelpers::GetMainDarkColor());
			m_lblProgress->SetForegroundColour(*wxWHITE);
			m_lblVersion->SetForegroundColour(*wxWHITE);
			m_lblCopyright->SetForegroundColour(*wxWHITE);
		}
		//==Thread==//
		m_trdStartup = std::jthread([&]()
			{
				std::this_thread::sleep_for(std::chrono::seconds(1));
				m_lblProgress->SetLabel("Loading configuration...");
				m_mainWindow->LoadConfig();
				m_lblProgress->SetLabel("Checking for updates...");
				m_mainWindow->CheckForUpdates();
				Close();
			});
	}
}