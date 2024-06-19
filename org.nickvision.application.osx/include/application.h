#ifndef APPLICATION_H
#define APPLICATION_H

#include <memory>
#include <wx/wx.h>
#include "controllers/mainwindowcontroller.h"

namespace Nickvision::Application::OSX
{
    /**
     * @brief The main OSX application point.
     */
    class Application : public wxApp
    {
    public:
        /**
         * @brief Constructs an Application.
         */
        Application() = default;
        /**
         * @brief Initializes the application.
         * @return True if the application was initialized, else false 
         */
        virtual bool OnInit();

    private:
        std::shared_ptr<Shared::Controllers::MainWindowController> m_controller;
    };
}

wxDECLARE_APP(Nickvision::Application::OSX::Application);

#endif //APPLICATION_H