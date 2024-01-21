#ifndef APPLICATION_H
#define APPLICATION_H

#include <string>
#include <memory>
#include <vector>
#include <adwaita.h>
#include "controllers/mainwindowcontroller.h"

namespace Nickvision::Application::GNOME
{
    /**
     * @brief The main GTK application point.
     */
    class Application
    {
    public:
        /**
         * @brief Constructs an Application.
         * @param argc The number of arguments passed to the application
         * @param argv The array of argument strings passed to the application
         */
        Application(int argc, char* argv[]) noexcept;
        /**
         * @brief Runs the application.
         * @brief This runs the gtk application loop.
         * @returns The return code from the gtk application 
         */
        int run() noexcept;

    private:
        /**
         * @brief Handles starting the application.
         * @param app The GtkApplication for the running app 
         */
        void onActivate(GtkApplication* app) noexcept;
        std::vector<char*> m_args;
        std::shared_ptr<Shared::Controllers::MainWindowController> m_controller;
        AdwApplication* m_adw;
    };
}

#endif //APPLCIATION_H