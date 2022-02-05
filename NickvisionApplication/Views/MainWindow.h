#pragma once

#include <wx/wx.h>
#include "../Controls/StatusBar.h"
#include "../Controls/InfoBar.h"

namespace NickvisionApplication::Views
{
	class MainWindow : public wxFrame
	{
	public:
		MainWindow();
		~MainWindow();

	private:
		enum IDs
		{
			WINDOW = 100,
			MENU_NEW_FILE,
			MENU_OPEN_FILE,
			MENU_EXIT,
			MENU_SETTINGS,
			MENU_CHECK_FOR_UPDATES,
			MENU_GITHUB_REPO,
			MENU_REPORT_A_BUG,
			MENU_CHANGELOG,
			MENU_ABOUT,
			TOOLBAR,
			STATUSBAR,
			INFOBAR,
			TOOL_OPEN_FILE,
			TOOL_SETTINGS,
			TOOL_CHECK_FOR_UPDATES,
			LBL_FIRST_NAME,
			TXT_FIRST_NAME,
			LBL_LAST_NAME,
			TXT_LAST_NAME
		};
		bool m_isLightTheme;
		//==Menu==//
		wxMenuBar* m_menuBar = nullptr;
		wxMenu* m_menuFile = nullptr;
		wxMenu* m_menuEdit = nullptr;
		wxMenu* m_menuHelp = nullptr;
		//==Main Window Controls==//
		wxToolBar* m_toolBar = nullptr;
		NickvisionApplication::Controls::StatusBar* m_statusBar = nullptr;
		NickvisionApplication::Controls::InfoBar* m_infoBar = nullptr;
		//==Layout==//
		wxBoxSizer* m_mainBox = nullptr;
		wxStaticText* m_lblFirstName = nullptr;
		wxTextCtrl* m_txtFirstName = nullptr;
		wxStaticText* m_lblLastName = nullptr;
		wxTextCtrl* m_txtLastName = nullptr;
		//==Slots==//
		void NewFile(wxCommandEvent& event);
		void OpenFile(wxCommandEvent& event);
		void Exit(wxCommandEvent& event);
		void Settings(wxCommandEvent& event);
		void CheckForUpdates(wxCommandEvent& event);
		void GitHubRepo(wxCommandEvent& event);
		void ReportABug(wxCommandEvent& event);
		void Changelog(wxCommandEvent& event);
		void About(wxCommandEvent& event);
	};
}