#include "application.h"
#include <vector>
#include "views/mainwindow.h"

using namespace Nickvision::Application::OSX::Views;
using namespace Nickvision::Application::Shared::Controllers;
using namespace Nickvision::Application::Shared::Models;

namespace Nickvision::Application::OSX
{
    bool Application::OnInit()
    {
        //Initialize wxWidgets
        if (!wxApp::OnInit())
        {
            return false;
        }
        //Create controller
        std::vector<std::string> args;
        for(int i = 0; i < wxApp::argc; i++)
        {
            args.push_back(wxApp::argv[i].ToStdString());
        }
        m_controller = std::make_shared<MainWindowController>(args);
        //macOS-ify the app
        wxMenuBar* menuBar{ new wxMenuBar() };
        wxMenuBar::MacSetCommonMenuBar(menuBar);
        //Set theme (TO BE ENABLED WITH WXWIDGETS 3.3.0)
        /*
        switch(m_controller->getTheme())
        {
        case Theme::Light:
            SetAppearance(wxApp::Appearance::Light);
            break;
        case Theme::Dark:
            SetAppearance(wxApp::Appearance::Dark);
            break;
        default:
            SetAppearance(wxApp::Appearance::System);
            break;
        }
        */
        //Create main window
        MainWindow* window{ new MainWindow(m_controller) }; //wxWidgets will clean the automatically
        SetTopWindow(window);
        window->Show(); 
        return true;
    }
}

wxIMPLEMENT_APP(Nickvision::Application::OSX::Application);