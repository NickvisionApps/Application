#include "HomePage.h"
#include <ctime>
#include "../Messenger.h"
#include "../Controls/AboutDialog.h"
#include "../../Models/AppInfo.h"

using namespace NickvisionApplication::Models;
using namespace NickvisionApplication::UI;
using namespace NickvisionApplication::UI::Controls;

namespace NickvisionApplication::UI::Views
{
	HomePage::HomePage(QWidget* parent) : QWidget{ parent }
	{
		m_ui.setupUi(this);
        //==AppName Label==//
        m_ui.lblAppName->setText(QString::fromStdString(AppInfo::getInstance().getName()));
		//==Welcome Label==//
		std::time_t timeNow{ std::time(0) };
		int timeNowHour{ std::localtime(&timeNow)->tm_hour };
        if (timeNowHour >= 0 && timeNowHour < 12)
        {
            m_ui.lblWelcome->setText("Good morning!");
        }
        else if (timeNowHour >= 12 && timeNowHour < 18)
        {
            m_ui.lblWelcome->setText("Good afternoon!");
        }
        else if (timeNowHour >= 18)
        {
            m_ui.lblWelcome->setText("Good evening!");
        }
	}

    void HomePage::on_btnSettings_clicked()
    {
        Messenger::getInstance().sendMessage("MainWindow.settings", nullptr);
    }

    void HomePage::on_btnCheckForUpdates_clicked()
    {
        Messenger::getInstance().sendMessage("MainWindow.checkForUpdates", nullptr);
    }

    void HomePage::on_btnAbout_clicked()
    {
        AboutDialog aboutDialog(this);
        aboutDialog.exec();
    }
}