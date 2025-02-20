#include "application.h"
#include <QStyleHints>
#include <oclero/qlementine/icons/QlementineIcons.hpp>

using namespace Nickvision::Application::Shared::Controllers;
using namespace Nickvision::Application::Shared::Models;

namespace Nickvision::Application::Qt
{
    Application::Application(int argc, char* argv[])
        : QApplication{ argc, argv },
        m_controller{ std::make_shared<MainWindowController>(std::vector<std::string>(argv, argv + argc)) },
        m_mainWindow{ nullptr },
        m_style{ new oclero::qlementine::QlementineStyle(this) },
        m_themeManager{ new oclero::qlementine::ThemeManager(m_style) }
    {
        oclero::qlementine::icons::initializeIconTheme();
        QApplication::setStyle(m_style);
        QIcon::setThemeName("qlementine");
        m_themeManager->loadDirectory(":/");
    }

    int Application::exec()
    {
        switch (m_controller->getTheme())
        {
        case Theme::Light:
            QApplication::styleHints()->setColorScheme(::Qt::ColorScheme::Light);
            m_themeManager->setCurrentTheme("Light");
            break;
        case Theme::Dark:
            QApplication::styleHints()->setColorScheme(::Qt::ColorScheme::Dark);
            m_themeManager->setCurrentTheme("Dark");
            break;
        default:
            QApplication::styleHints()->unsetColorScheme();
            m_themeManager->setCurrentTheme(QApplication::styleHints()->colorScheme() == ::Qt::ColorScheme::Light ? "Light" : "Dark");
            break;
        }
        m_mainWindow = std::make_shared<Views::MainWindow>(m_controller, m_themeManager);
        m_mainWindow->show();
        return QApplication::exec();
    }
}
