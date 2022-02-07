#include "SettingsDialog.h"
#include <dwmapi.h>

namespace NickvisionApplication::Views
{
	SettingsDialog::SettingsDialog(wxWindow* parent, bool isLightTheme) : wxDialog(parent, IDs::DIALOG, _("Settings"), wxDefaultPosition, { 600, 500 })
	{
		//==Window Settings==//
		CenterOnParent();
		Connect(IDs::DIALOG, wxEVT_CLOSE_WINDOW, wxCloseEventHandler(SettingsDialog::OnClose));
		//==Tree==//
		m_mainTree = new wxTreeCtrl(this, IDs::TREE);
		m_mainTree->SetMaxSize({ 200, -1 });
		m_treeNodeRoot = m_mainTree->AddRoot(_("Settings"));
		m_treeNodeUI = m_mainTree->AppendItem(m_treeNodeRoot, _("User Interface"));
		m_treeNodeApp = m_mainTree->AppendItem(m_treeNodeRoot, _("Application"));
		m_mainTree->ExpandAll();
		Connect(IDs::TREE, wxEVT_TREE_SEL_CHANGED, wxTreeEventHandler(SettingsDialog::TreeSelectionChanged));
		//==Root Page==//
		m_pageRoot = new wxPanel(this, IDs::PAGE_ROOT);
		m_boxRoot = new wxBoxSizer(wxVERTICAL);
		m_lblRoot = new wxStaticText(m_pageRoot, IDs::LBL_ROOT, _("Please select a category to edit."));
		m_pageRoot->SetSizer(m_boxRoot);
		//==UI Page==//
		m_pageUI = new wxPanel(this, IDs::PAGE_UI);
		m_boxUI = new wxBoxSizer(wxVERTICAL);
		m_chkPreferLightTheme = new wxCheckBox(m_pageUI, IDs::CHK_PREFER_LIGHT_THEME, _("Prefer Light Theme"));
		m_chkPreferLightTheme->SetToolTip(new wxToolTip(_("If checked, Application will use a light theme for the UI, else a dark theme. (App restart required)")));
		m_pageUI->SetSizer(m_boxUI);
		//==App Page==//
		m_pageApp = new wxPanel(this, IDs::PAGE_APP);
		m_boxApp = new wxBoxSizer(wxVERTICAL);
		m_pageApp->SetSizer(m_boxApp);
		//==Layout==//
		m_mainBox = new wxBoxSizer(wxHORIZONTAL);
		m_mainBox->Add(m_mainTree, 1, wxEXPAND);
		SetSizer(m_mainBox);
		//==Theme==//
		if (isLightTheme) //Light
		{
			//Dialog
			SetBackgroundColour({ 251, 251, 251 });
			//Tree
			m_mainTree->SetBackgroundColour({ 243, 243, 243 });
		}
		else //Dark
		{
			//Win32
			BOOL enabled = TRUE;
			DwmSetWindowAttribute(GetHWND(), DWMWA_USE_IMMERSIVE_DARK_MODE, &enabled, sizeof(enabled));
			SetWindowTheme(GetHWND(), L"Explorer", NULL);
			//Dialog
			SetBackgroundColour({ 43, 43, 43 });
			//Tree
			m_mainTree->SetBackgroundColour({ 32, 32, 32 });
			m_mainTree->SetForegroundColour(*wxWHITE);
			//Other Controls
			m_lblRoot->SetForegroundColour(*wxWHITE);
			m_chkPreferLightTheme->SetForegroundColour(*wxWHITE);
		}
		//==Load Config==//
		m_chkPreferLightTheme->SetValue(m_configuration.PreferLightTheme());
	}

	void SettingsDialog::OnClose(wxCloseEvent& WXUNUSED(event))
	{
		//==Save Config==//
		m_configuration.SetPreferLightTheme(m_chkPreferLightTheme->GetValue());
		m_configuration.Save();
		//==Finish==//
		Destroy();
	}

	void SettingsDialog::TreeSelectionChanged(wxTreeEvent& event)
	{
		wxTreeItemId selectedNode = event.GetItem();
		if (m_mainBox->GetItemCount() == 2)
		{
			m_mainBox->Remove(1);
		}
		if (selectedNode == m_treeNodeRoot)
		{
			m_mainBox->Add(m_pageRoot, 1, wxEXPAND | wxALL, 6);
			m_pageRoot->Show();
			m_pageUI->Hide();
			m_pageApp->Hide();
		}
		else if (selectedNode == m_treeNodeUI)
		{
			m_mainBox->Add(m_pageUI, 1, wxEXPAND | wxALL, 6);
			m_pageRoot->Hide();
			m_pageUI->Show();
			m_pageApp->Hide();
		}
		else if (selectedNode == m_treeNodeApp)
		{
			m_mainBox->Add(m_pageApp, 1, wxEXPAND | wxALL, 6);
			m_pageRoot->Hide();
			m_pageUI->Hide();
			m_pageApp->Show();
		}
		m_mainBox->Layout();
	}
}