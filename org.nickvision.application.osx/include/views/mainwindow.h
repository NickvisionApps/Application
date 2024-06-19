#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <memory>
#include <wx/wx.h>
#include "controllers/mainwindowcontroller.h"

namespace Nickvision::Application::OSX::Views
{
    class MainWindow : public wxFrame
    {
    public:
        MainWindow(const std::shared_ptr<Shared::Controllers::MainWindowController>& controller);

    private:
        std::shared_ptr<Shared::Controllers::MainWindowController> m_controller;
    };
}

#endif //MAINWINDOW_H