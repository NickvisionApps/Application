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
        bool Show(bool show = true) override;

    private:
        /**
         * @brief Handles when the window is closed.
         * @param event wxCloseEvent
         */
        void onClose(wxCloseEvent& event);
        std::shared_ptr<Shared::Controllers::MainWindowController> m_controller;
    };
}

#endif //MAINWINDOW_H