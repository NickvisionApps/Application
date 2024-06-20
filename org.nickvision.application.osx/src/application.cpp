#include "application.h"
#include <vector>
#include "views/mainwindow.h"

using namespace Nickvision::Application::OSX::Views;
using namespace Nickvision::Application::Shared::Controllers;

namespace Nickvision::Application::OSX
{
    bool Application::OnInit()
    {
        //Create controller
        std::vector<std::string> args;
        for(int i = 0; i < wxApp::argc; i++)
        {
            args.push_back(wxApp::argv[i].ToStdString());
        }
        m_controller = std::make_shared<MainWindowController>(args);
        m_controller->getAppInfo().setChangelog("- Initial Release");
        //macOS-ify the app
        wxMenuBar* menuBar{ new wxMenuBar() };
        wxMenuBar::MacSetCommonMenuBar(menuBar);
        //Create main window
        (new MainWindow(m_controller))->Show(); //wxWidgets will clean the automatically
        return true;
    }
}

wxIMPLEMENT_APP(Nickvision::Application::OSX::Application);