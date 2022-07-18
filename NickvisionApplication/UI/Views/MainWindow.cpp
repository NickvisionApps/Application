#include "MainWindow.h"
#include <QIcon>
#include <QMessageBox>
#include "SettingsDialog.h"
#include "../Controls/AboutDialog.h"
#include "../Controls/ProgressDialog.h"
#include "../../Helpers/ThemeHelpers.h"
#include "../../Models/AppInfo.h"

using namespace NickvisionApplication::Helpers;
using namespace NickvisionApplication::Models;
using namespace NickvisionApplication::UI::Controls;

namespace NickvisionApplication::UI::Views
{
	MainWindow::MainWindow(QWidget* parent) : QMainWindow{ parent }, m_updater{ "https://raw.githubusercontent.com/nlogozzo/NickvisionApplication/main/UpdateConfig.json", { "2022.7.0" } }
	{
		m_ui.setupUi(this);
		//==Window Settings==//
		setWindowTitle(QString::fromStdString(AppInfo::getInstance().getName()));
		setWindowIcon(QIcon(":/Resources/icon.ico"));
		//==Pages==//
		m_ui.viewStack->addWidget(&m_homePage);
		//==Theme==//
		if (Configuration::getInstance().getTheme() == Theme::Light)
		{
			QApplication::setPalette(ThemeHelpers::getLightPalette());
			m_currentTheme = Theme::Light;
		}
		else
		{
			QApplication::setPalette(ThemeHelpers::getDarkPalette());
			m_currentTheme = Theme::Dark;
		}
		ThemeHelpers::applyWin32Theming(this);
	}

	void MainWindow::on_menuNewFile_triggered()
	{

	}

	void MainWindow::on_menuOpenFile_triggered()
	{

	}

	void MainWindow::on_menuCloseFile_triggered()
	{

	}

	void MainWindow::on_menuExit_triggered()
	{
		close();
	}

	void MainWindow::on_menuSettings_triggered()
	{
		SettingsDialog settingsDialog{ this };
		settingsDialog.exec();
		refreshTheme();
	}

	void MainWindow::on_menuCheckForUpdates_triggered()
	{
		ProgressDialog checkingDialog{ this, "Checking for updates...", [&]() {  m_updater.checkForUpdates(); } };
		checkingDialog.exec();
		if (m_updater.getUpdateAvailable())
		{
			QMessageBox msgUpdate{ QMessageBox::Icon::Information, "Update Available", QString::fromStdString("===V" + m_updater.getLatestVersion().toString() + " Changelog===\n" + m_updater.getChangelog() + "\n\nNickvisionApplication will automatically download and install the update. Please save all work before continuing. Are you ready to update?"), QMessageBox::StandardButton::Yes | QMessageBox::StandardButton::No, this};
			ThemeHelpers::applyWin32Theming(&msgUpdate);
			int result = msgUpdate.exec();
			if (result == QMessageBox::StandardButton::Yes)
			{
				ProgressDialog updateDialog{ this, "Downloading and installing the update...", [&]() { m_updater.windowsUpdate(this); } };
				updateDialog.exec();
				if (!m_updater.getUpdateSuccessful())
				{
					QMessageBox msgError{ QMessageBox::Icon::Critical, "Error", "There was an error downloading and installing the update. Please try again.\nIf the error continues, file a bug report.", QMessageBox::StandardButton::Ok, this };
					ThemeHelpers::applyWin32Theming(&msgError);
					msgError.exec();
				}
			}
		}
		else
		{
			QMessageBox msgNoUpdate{ QMessageBox::Icon::Critical, "Update", "There is no update is available at this time. Please try again later.", QMessageBox::StandardButton::Ok, this };
			ThemeHelpers::applyWin32Theming(&msgNoUpdate);
			msgNoUpdate.exec();
		}
	}

	void MainWindow::on_menuAbout_triggered()
	{
		AboutDialog aboutDialog(this);
		aboutDialog.exec();
	}

	void MainWindow::refreshTheme()
	{
		if (Configuration::getInstance().getTheme() != m_currentTheme)
		{
			if (Configuration::getInstance().getTheme() == Theme::Light)
			{
				QApplication::setPalette(ThemeHelpers::getLightPalette());
				m_currentTheme = Theme::Light;
			}
			else
			{
				QApplication::setPalette(ThemeHelpers::getDarkPalette());
				m_currentTheme = Theme::Dark;
			}
			ThemeHelpers::applyWin32Theming(this);
		}
	}
}