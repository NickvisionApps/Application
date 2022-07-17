#include "AboutDialog.h"
#include <QDesktopServices>
#include "../../Helpers/ThemeHelpers.h"
#include "../../Models/AppInfo.h"

using namespace NickvisionApplication::Helpers;
using namespace NickvisionApplication::Models;

namespace NickvisionApplication::UI::Views
{
	AboutDialog::AboutDialog(QWidget* parent) : QDialog(parent)
	{
		m_ui.setupUi(this);
		//==Theme==//
		ThemeHelpers::applyWin32Theming(this);
		//==App Info==//
		AppInfo& appInfo{ AppInfo::getInstance() };
		m_ui.lblAppName->setText(QString::fromStdString(appInfo.getName()));
		m_ui.lblDescription->setText(QString::fromStdString(appInfo.getDescription()));
		m_ui.lblVersion->setText(QString::fromStdString("Version " + appInfo.getVersion()));
		m_ui.lblChangelog->setText(QString::fromStdString(appInfo.getChangelog()));
	}

	void AboutDialog::on_btnGitHubRepo_clicked()
	{
		QDesktopServices::openUrl({ "https://github.com/nlogozzo/NickvisionApplication" });
	}

	void AboutDialog::on_btnReportABug_clicked()
	{
		QDesktopServices::openUrl({ "https://github.com/nlogozzo/NickvisionApplication/issues/new" });
	}

	void AboutDialog::on_btnClose_clicked()
	{
		close();
	}
}