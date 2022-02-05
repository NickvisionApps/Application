#pragma once

#include <wx/wx.h>
#include <wx/treectrl.h>
#include "../Models/Configuration.h"

namespace NickvisionApplication::Views
{
	class SettingsDialog : public wxDialog
	{
	public:
		SettingsDialog(wxWindow* parent, bool isLightTheme);
		~SettingsDialog();

	private:
		enum IDs
		{
			DIALOG = 300,
			TREE,
			PAGE_ROOT,
			LBL_ROOT,
			PAGE_UI,
			CHK_PREFER_LIGHT_THEME,
			PAGE_APP
		};
		NickvisionApplication::Models::Configuration m_configuration;
		//==Layout==//
		wxBoxSizer* m_mainBox = nullptr;
		//Tree
		wxTreeCtrl* m_mainTree = nullptr;
		wxTreeItemId m_treeNodeRoot;
		wxTreeItemId m_treeNodeUI;
		wxTreeItemId m_treeNodeApp;
		//Root Page
		wxPanel* m_pageRoot = nullptr;
		wxBoxSizer* m_boxRoot = nullptr;
		wxStaticText* m_lblRoot = nullptr;
		//UI Page
		wxPanel* m_pageUI = nullptr;
		wxBoxSizer* m_boxUI = nullptr;
		wxCheckBox* m_chkPreferLightTheme = nullptr;
		//App Page
		wxPanel* m_pageApp = nullptr;
		wxBoxSizer* m_boxApp = nullptr;
		//==Slots==//
		void TreeSelectionChanged(wxTreeEvent& event);
	};
}

