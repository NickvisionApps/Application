#include "SettingsDialog.h"
#include "../Helpers/ThemeHelpers.h"

namespace NickvisionApplication::Views
{
	using namespace NickvisionApplication::Helpers;

	SettingsDialog::SettingsDialog(wxWindow* parent, bool isLightTheme) : wxDialog(parent, IDs::DIALOG, _("Settings"), wxDefaultPosition, { 600, 500 }, wxCAPTION)
	{
		//==Window Settings==//
		CenterOnParent();
		Connect(IDs::DIALOG, wxEVT_CLOSE_WINDOW, wxCloseEventHandler(SettingsDialog::OnClose));
		//==Tree==//
		m_mainTree = new wxTreeCtrl(this, IDs::TREE, wxDefaultPosition, wxDefaultSize, wxTR_DEFAULT_STYLE | wxBORDER_NONE);
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
		m_chkPreferLightTheme->SetToolTip(new wxToolTip(_("If checked, Application will use a light theme for the UI, else a dark theme.")));
		m_pageUI->SetSizer(m_boxUI);
		//==App Page==//
		m_pageApp = new wxPanel(this, IDs::PAGE_APP);
		m_boxApp = new wxBoxSizer(wxVERTICAL);
		m_pageApp->SetSizer(m_boxApp);
		//==Save Button==//
		m_btnSave = new wxButton(this, IDs::BTN_SAVE, "Save", wxDefaultPosition, { -1, 30 }, wxBORDER_NONE);
		Connect(IDs::BTN_SAVE, wxEVT_BUTTON, wxCommandEventHandler(SettingsDialog::Save));
		//==Layout==//
		m_settingsBox = new wxBoxSizer(wxHORIZONTAL);
		m_settingsBox->Add(m_mainTree, 1, wxEXPAND);
		m_mainBox = new wxBoxSizer(wxVERTICAL);
		m_mainBox->Add(m_settingsBox, 1, wxEXPAND);
		m_mainBox->Add(m_btnSave, 0, wxEXPAND);
		SetSizer(m_mainBox);
		//==Theme==//
		if (isLightTheme) //Light
		{
			//Win32
			ThemeHelpers::ApplyWin32LightMode(this);
			//Dialog
			SetBackgroundColour(ThemeHelpers::GetSecondaryLightColor());
			//Tree
			m_mainTree->SetBackgroundColour(ThemeHelpers::GetMainLightColor());
			//Save Button
			m_btnSave->SetBackgroundColour(ThemeHelpers::GetTertiaryLightColor());
		}
		else //Dark
		{
			//Win32
			ThemeHelpers::ApplyWin32DarkMode(this);
			//Dialog
			SetBackgroundColour(ThemeHelpers::GetSecondaryDarkColor());
			//Tree
			m_mainTree->SetBackgroundColour(ThemeHelpers::GetMainDarkColor());
			m_mainTree->SetForegroundColour(*wxWHITE);
			//Settings Controls
			m_lblRoot->SetForegroundColour(*wxWHITE);
			m_chkPreferLightTheme->SetForegroundColour(*wxWHITE);
			//Save Button
			m_btnSave->SetBackgroundColour(ThemeHelpers::GetTertiaryDarkColor());
			m_btnSave->SetForegroundColour(*wxWHITE);
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

	void SettingsDialog::Save(wxCommandEvent& WXUNUSED(event))
	{
		Close();
	}

	void SettingsDialog::TreeSelectionChanged(wxTreeEvent& event)
	{
		wxTreeItemId selectedNode = event.GetItem();
		if (m_settingsBox->GetItemCount() == 2)
		{
			m_settingsBox->Remove(1);
		}
		if (selectedNode == m_treeNodeRoot)
		{
			m_settingsBox->Add(m_pageRoot, 1, wxEXPAND | wxALL, 6);
			m_pageRoot->Show();
			m_pageUI->Hide();
			m_pageApp->Hide();
		}
		else if (selectedNode == m_treeNodeUI)
		{
			m_settingsBox->Add(m_pageUI, 1, wxEXPAND | wxALL, 6);
			m_pageRoot->Hide();
			m_pageUI->Show();
			m_pageApp->Hide();
		}
		else if (selectedNode == m_treeNodeApp)
		{
			m_settingsBox->Add(m_pageApp, 1, wxEXPAND | wxALL, 6);
			m_pageRoot->Hide();
			m_pageUI->Hide();
			m_pageApp->Show();
		}
		m_settingsBox->Layout();
	}
}