#include <QApplication>
#include "Models/AppInfo.h"
#include "UI/Views/MainWindow.h"

using namespace NickvisionApplication::Models;
using namespace NickvisionApplication::UI::Views;

int main(int argc, char *argv[])
{
    //==App Info==//
    AppInfo& appInfo = AppInfo::getInstance();
    appInfo.setName("NickvisionApplication");
    appInfo.setDescription("A template for creating Nickvision applications.");
    appInfo.setVersion("2022.7.0-dev");
    appInfo.setChangelog("- Application rewrite with C++ and QT");
    appInfo.setGitHubRepo("https://github.com/nlogozzo/NickvisionApplication");
    appInfo.setIssueTracker("https://github.com/nlogozzo/NickvisionApplication/issues/new");
    //==Vars==//
    QApplication application{ argc, argv };
    MainWindow mainWindow;
    //==App Settings==//
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling, true);
    QApplication::setStyle("fusion");
    //==Run App==//
    mainWindow.showMaximized();
    return application.exec();
}
