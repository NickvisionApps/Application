#include "application.h"
#include <vector>
#include "views/mainwindow.h"

using namespace Nickvision::Application::OSX::Views;
using namespace Nickvision::Application::Shared::Controllers;

namespace Nickvision::Application::OSX
{
    bool Application::OnInit()
    {
        std::vector<std::string> args;
        for(int i = 0; i < wxApp::argc; i++)
        {
            args.push_back(wxApp::argv[i].ToStdString());
        }
        m_controller = std::make_shared<MainWindowController>(args);
        m_controller->getAppInfo().setChangelog("- Initial Release");
        (new MainWindow(m_controller))->Show(); //wxWidgets will clean the automatically
        return true;
    }
}

wxIMPLEMENT_APP(Nickvision::Application::OSX::Application);