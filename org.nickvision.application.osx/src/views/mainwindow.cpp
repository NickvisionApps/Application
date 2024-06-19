#include "views/mainwindow.h"
#include <libnick/localization/gettext.h>

using namespace Nickvision::Application::Shared::Controllers;

namespace Nickvision::Application::OSX::Views
{
    MainWindow::MainWindow(const std::shared_ptr<MainWindowController>& controller)
        : wxFrame{ nullptr, wxID_ANY, controller->getAppInfo().getName() },
        m_controller{ controller }
    {
        
    }
}