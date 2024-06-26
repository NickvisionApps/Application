#include "views/mainwindow.h"
#include <libnick/localization/gettext.h>

using namespace Nickvision::App;
using namespace Nickvision::Application::Shared::Controllers;

namespace Nickvision::Application::OSX::Views
{
    MainWindow::MainWindow(const std::shared_ptr<MainWindowController>& controller)
        : wxFrame{ nullptr, wxID_ANY, controller->getAppInfo().getShortName() },
        m_controller{ controller }
    {
        Bind(wxEVT_CLOSE_WINDOW, &MainWindow::onClose, this);
    }

    bool MainWindow::Show(bool show)
    {
        WindowGeometry geometry{ m_controller->startup() };
        SetSize(geometry.getWidth(), geometry.getHeight());
        if(geometry.isMaximized())
        {
            Maximize();
        }
        return wxFrame::Show(show);
    }

    void MainWindow::onClose(wxCloseEvent& event)
    {
        if(event.CanVeto() && !m_controller->canShutdown())
        {
            event.Veto();
            return;
        }
        wxSize size{ GetSize() };
        m_controller->shutdown({ size.GetWidth(), size.GetHeight(), IsMaximized() });
        event.Skip();
    }
}