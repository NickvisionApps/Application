#include "SettingsDialog.h"
#include "../../Helpers/ThemeHelpers.h"

using namespace NickvisionApplication::Helpers;
using namespace NickvisionApplication::Models;

namespace NickvisionApplication::UI::Views
{
	SettingsDialog::SettingsDialog(QWidget* parent) : QDialog{ parent }, m_configuration{ Configuration::getInstance() }
	{
		m_ui.setupUi(this);
		//==Theme==//
		ThemeHelpers::applyWin32Theming(this);
		//==Load Config==//
		m_ui.chkPreferLightTheme->setChecked(m_configuration.getTheme() == Theme::Light);
	}

	void SettingsDialog::on_treeNav_itemSelectionChanged()
	{
		QString selectedItemItem{ m_ui.treeNav->currentItem()->text(0)};
		if (selectedItemItem == "User Interface")
		{
			m_ui.viewStack->setCurrentIndex(0);
		}
		else if (selectedItemItem == "Application")
		{
			m_ui.viewStack->setCurrentIndex(1);
		}
	}

	void SettingsDialog::on_btnSave_clicked()
	{
		m_configuration.setTheme(m_ui.chkPreferLightTheme->isChecked() ? Theme::Light : Theme::Dark);
		m_configuration.save();
		close();
	}

	void SettingsDialog::on_btnCancel_clicked()
	{
		close();
	}
}
