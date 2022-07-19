#include "HomePage.h"
#include <ctime>
#include "../../Models/AppInfo.h"

using namespace NickvisionApplication::Models;

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
}