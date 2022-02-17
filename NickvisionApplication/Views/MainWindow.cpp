#include "MainWindow.h"
#include <fstream>
#include <wx/busyinfo.h>
#include "../Models/Configuration.h"
#include "../Helpers/ThemeHelpers.h"
#include "SettingsDialog.h"

namespace NickvisionApplication::Views
{
	using namespace NickvisionApplication::Models;
	using namespace NickvisionApplication::Models::Update;
	using namespace NickvisionApplication::Helpers;
	using namespace NickvisionApplication::Controls;

	MainWindow::MainWindow() : wxFrame(nullptr, IDs::WINDOW, "NickvisionApplication", wxDefaultPosition, wxSize(800, 600)), m_isLightTheme(false), m_updater("https://raw.githubusercontent.com/nlogozzo/NickvisionApplication/main/UpdateConfig.json", { "2022.2.0" })
	{
		//==Window Settings==//
		SetIcon(wxICON(APP_ICON));
		Maximize();
		Connect(IDs::WINDOW, wxEVT_CLOSE_WINDOW, wxCloseEventHandler(MainWindow::OnClose));
		//==Menu==//
		m_menuBar = new wxMenuBar();
		//File
		m_menuFile = new wxMenu();
		m_menuFile->Append(IDs::MENU_NEW_FILE, _("&New File\tCtrl+N"));
		Connect(IDs::MENU_NEW_FILE, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainWindow::NewFile));
		m_menuFile->Append(IDs::MENU_OPEN_FILE, _("&Open File\tCtrl+O"));
		Connect(IDs::MENU_OPEN_FILE, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainWindow::OpenFile));
		m_menuFile->AppendSeparator();
		m_menuFile->Append(IDs::MENU_EXIT, _("E&xit\tAlt+F4"));
		Connect(IDs::MENU_EXIT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainWindow::Exit));
		m_menuBar->Append(m_menuFile, _("&File"));
		//Edit
		m_menuEdit = new wxMenu();
		m_menuEdit->Append(IDs::MENU_SETTINGS, _("&Settings\tCtrl+."));
		Connect(IDs::MENU_SETTINGS, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainWindow::Settings));
		m_menuBar->Append(m_menuEdit, _("&Edit"));
		//Help
		m_menuHelp = new wxMenu();
		m_menuHelp->Append(IDs::MENU_UPDATE, _("&Update"));
		Connect(IDs::MENU_UPDATE, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainWindow::Update));
		m_menuHelp->AppendSeparator();
		m_menuHelp->Append(IDs::MENU_GITHUB_REPO, _("&GitHub Repo"));
		Connect(IDs::MENU_GITHUB_REPO, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainWindow::GitHubRepo));
		m_menuHelp->Append(IDs::MENU_REPORT_A_BUG, _("&Report a Bug"));
		Connect(IDs::MENU_REPORT_A_BUG, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainWindow::ReportABug));
		m_menuHelp->AppendSeparator();
		m_menuHelp->Append(IDs::MENU_CHANGELOG, _("&Changelog"));
		Connect(IDs::MENU_CHANGELOG, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainWindow::Changelog));
		m_menuHelp->Append(IDs::MENU_ABOUT, _("&About Application\tF1"));
		Connect(IDs::MENU_ABOUT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainWindow::About));
		m_menuBar->Append(m_menuHelp, _("&Help"));
		SetMenuBar(m_menuBar);
		//==ToolBar==//
		m_toolBar = new wxToolBar(this, IDs::TOOLBAR, wxDefaultPosition, wxDefaultSize, wxTB_HORIZONTAL);
		m_toolBar->AddTool(IDs::TOOL_OPEN_FILE, "", wxBitmap("OPENED_FOLDER", wxBITMAP_TYPE_PNG_RESOURCE))->SetShortHelp(_("Open Folder"));
		Connect(IDs::TOOL_OPEN_FILE, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(MainWindow::OpenFile));
		m_toolBar->AddStretchableSpace();
		m_toolBar->AddTool(IDs::TOOL_SETTINGS, "", wxBitmap("SETTINGS", wxBITMAP_TYPE_PNG_RESOURCE))->SetShortHelp(_("Settings"));
		Connect(IDs::TOOL_SETTINGS, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(MainWindow::Settings));
		m_toolBar->Realize();
		SetToolBar(m_toolBar);
		//==StatusBar==//
		m_statusBar = new StatusBar(this, IDs::STATUSBAR);
		SetStatusBar(m_statusBar);
		//==InfoBar==//
		m_infoBar = new InfoBar(this, IDs::INFOBAR);
		//==First Name==//
		m_lblFirstName = new wxStaticText(this, IDs::LBL_FIRST_NAME, _("First Name"));
		m_txtFirstName = new wxTextCtrl(this, IDs::TXT_FIRST_NAME, "", wxDefaultPosition, { 320, 24 }, wxBORDER_NONE);
		m_txtFirstName->SetHint(_("Enter first name here"));
		//==Last Name==//
		m_lblLastName = new wxStaticText(this, IDs::LBL_LAST_NAME, _("Last Name"));
		m_txtLastName = new wxTextCtrl(this, IDs::TXT_LAST_NAME, "", wxDefaultPosition, { 320, 24 }, wxBORDER_NONE);
		m_txtLastName->SetHint(_("Enter last name here"));
		//==Layout==//
		m_mainBox = new wxBoxSizer(wxVERTICAL);
		m_mainBox->Add(m_infoBar, 0, wxEXPAND);
		m_mainBox->Add(m_lblFirstName, 0, wxLEFT | wxTOP, 6);
		m_mainBox->Add(m_txtFirstName, 0, wxLEFT | wxTOP, 6);
		m_mainBox->Add(m_lblLastName, 0, wxLEFT | wxTOP, 6);
		m_mainBox->Add(m_txtLastName, 0, wxLEFT | wxTOP, 6);
		SetSizer(m_mainBox);
	}

	void MainWindow::SetIsLightTheme(bool isLightTheme)
	{
		m_isLightTheme = isLightTheme;
		if (m_isLightTheme)
		{
			//Win32
			ThemeHelpers::ApplyWin32LightMode(this);
			//Window
			SetBackgroundColour(ThemeHelpers::GetMainLightColor());
			//ToolBar
			m_toolBar->SetBackgroundColour(ThemeHelpers::GetSecondaryLightColor());
			m_toolBar->SetForegroundColour(*wxBLACK);
			//StatusBar
			m_statusBar->SetIsLightTheme(true);
			//InfoBar
			m_infoBar->SetIsLightTheme(true);
			//First Name
			m_lblFirstName->SetForegroundColour(*wxBLACK);
			m_txtFirstName->SetBackgroundColour(ThemeHelpers::GetSecondaryLightColor());
			m_txtFirstName->SetForegroundColour(*wxBLACK);
			//Last Name
			m_lblLastName->SetForegroundColour(*wxBLACK);
			m_txtLastName->SetBackgroundColour(ThemeHelpers::GetSecondaryLightColor());
			m_txtLastName->SetForegroundColour(*wxBLACK);
		}
		else
		{
			//Win32
			ThemeHelpers::ApplyWin32DarkMode(this);
			//Window
			SetBackgroundColour(ThemeHelpers::GetMainDarkColor());
			//ToolBar
			m_toolBar->SetBackgroundColour(ThemeHelpers::GetSecondaryDarkColor());
			m_toolBar->SetForegroundColour(*wxWHITE);
			//StatusBar
			m_statusBar->SetIsLightTheme(false);
			//InfoBar
			m_infoBar->SetIsLightTheme(false);
			//First Name
			m_lblFirstName->SetForegroundColour(*wxWHITE);
			m_txtFirstName->SetBackgroundColour(ThemeHelpers::GetSecondaryDarkColor());
			m_txtFirstName->SetForegroundColour(*wxWHITE);
			//Last Name
			m_lblLastName->SetForegroundColour(*wxWHITE);
			m_txtLastName->SetBackgroundColour(ThemeHelpers::GetSecondaryDarkColor());
			m_txtLastName->SetForegroundColour(*wxWHITE);
		}
		m_infoBar->Dismiss();
		Refresh();
	}

	void MainWindow::LoadConfig()
	{
		Configuration configuration;
		SetIsLightTheme(configuration.GetPreferLightTheme());
	}

	void MainWindow::CheckForUpdates()
	{
		m_updater.CheckForUpdates();
		if (m_updater.UpdateAvailable())
		{
			m_infoBar->ShowMessage(_("There is an update available. Please run the update command in the help menu to download and install the update."), wxICON_INFORMATION);
		}
	}

	void MainWindow::OnClose(wxCloseEvent& event)
	{
		//==Save Config==//
		Configuration configuration;
		configuration.Save();
		//==Finish==//
		event.Skip();
	}

	void MainWindow::NewFile(wxCommandEvent& WXUNUSED(event))
	{
		wxFileDialog newFileDialog(this, _("New File"), "", "", "Text Files (*.txt)|*.txt", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
		if (newFileDialog.ShowModal() == wxID_OK)
		{
			std::string path(newFileDialog.GetPath());
			m_statusBar->SetMessage(path);
			std::ofstream file(path);
			if (file.is_open())
			{
				file << "" << std::endl;
			}
		}
	}

	void MainWindow::OpenFile(wxCommandEvent& WXUNUSED(event))
	{
		wxFileDialog openFileDialog(this, _("Open File"), "", "", "Text Files (*.txt)|*.txt", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
		if (openFileDialog.ShowModal() == wxID_OK)
		{
			m_statusBar->SetMessage(openFileDialog.GetPath());
		}
	}

	void MainWindow::Exit(wxCommandEvent& WXUNUSED(event))
	{
		Close();
	}

	void MainWindow::Settings(wxCommandEvent& WXUNUSED(event))
	{
		SettingsDialog settingsDialog(this, m_isLightTheme);
		settingsDialog.ShowModal();
		Configuration configuration;
		if (configuration.GetPreferLightTheme() != m_isLightTheme)
		{
			SetIsLightTheme(configuration.GetPreferLightTheme());
		}
	}

	void MainWindow::Update(wxCommandEvent& WXUNUSED(event))
	{
		if (m_updater.UpdateAvailable())
		{
			int result = wxMessageBox(_("Update Available\n\n===V" + m_updater.GetLatestVersion()->ToString() + " Changelog===\n" + m_updater.GetChangelog() + "\n\nNickvisionApplication will automatically download and install the update. Please save all work before continuing. Are you ready to update?"), _("Update"), wxICON_INFORMATION | wxYES_NO, this);
			if (result == wxYES)
			{
				bool updateSuccess = false;
				wxBusyInfo busyUpdating(wxBusyInfoFlags()
					.Parent(this)
					.Title(_("<b>Please Wait</b>"))
					.Text(_("Downloading and installing the update..."))
					.Background(m_isLightTheme ? *wxWHITE : *wxBLACK)
					.Foreground(m_isLightTheme ? *wxBLACK : *wxWHITE)
					.Transparency(4 * wxALPHA_OPAQUE / 5));
				updateSuccess = m_updater.Update();
				busyUpdating.~wxBusyInfo();
				if (!updateSuccess)
				{
					m_infoBar->ShowMessage(_("There was an error downloading and install the update. Please try again and if the error continues, file a bug report."), wxICON_ERROR);
				}
			}
		}
		else
		{
			m_infoBar->ShowMessage(_("There is no update available at this time. Please try again later."), wxICON_ERROR);
		}
	}

	void MainWindow::GitHubRepo(wxCommandEvent& WXUNUSED(event))
	{
		wxLaunchDefaultBrowser("https://github.com/nlogozzo/NickvisionApplication");
	}

	void MainWindow::ReportABug(wxCommandEvent& WXUNUSED(event))
	{
		wxLaunchDefaultBrowser("https://github.com/nlogozzo/NickvisionApplication/issues/new");
	}

	void MainWindow::Changelog(wxCommandEvent& WXUNUSED(event))
	{
		wxMessageBox(_("What's New?\n\n- Initial Release"), _("Changelog"), wxICON_INFORMATION, this);
	}

	void MainWindow::About(wxCommandEvent& WXUNUSED(event))
	{
		wxMessageBox(_("About NickvisionApplication\n\nVersion 2022.2.0\nA template for creating Nickvision applications.\n\nBuilt with C++, wxWidgets, and Icons8\n(C) Nickvision 2021-2022"), _("About"), wxICON_INFORMATION, this);
	}
}