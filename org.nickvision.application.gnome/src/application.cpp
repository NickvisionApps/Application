#include "application.h"
#include "views/mainwindow.h"

using namespace Nickvision::Application::Shared::Controllers;
using namespace Nickvision::Application::Shared::Models;

namespace Nickvision::Application::GNOME
{
    Application::Application(int argc, char* argv[]) noexcept
        : m_controller{ std::make_shared<MainWindowController>() },
        m_adw{ adw_application_new(m_controller->getAppInfo().getId().c_str(), G_APPLICATION_DEFAULT_FLAGS) }
    {
        m_args.reserve(static_cast<size_t>(argc));
        for(int i = 0; i < argc; i++)
        {
            m_args.push_back(argv[i]);
        }
        m_controller->getAppInfo().setChangelog("- Initial Release");
        g_resources_register(g_resource_load(std::string(m_controller->getAppInfo().getId() + ".gresource").c_str(), nullptr));
        g_signal_connect(m_adw, "activate", G_CALLBACK(+[](GtkApplication* app, gpointer data){ reinterpret_cast<Application*>(data)->onActivate(app); }), this);
    }

    int Application::run() noexcept
    {
        return g_application_run(G_APPLICATION(m_adw), static_cast<int>(m_args.size()), &m_args[0]);
    }

    void Application::onActivate(GtkApplication* app) noexcept
    {
        switch (m_controller->getTheme())
        {
        case Theme::Light:
            adw_style_manager_set_color_scheme(adw_style_manager_get_default(), ADW_COLOR_SCHEME_FORCE_LIGHT);
            break;
        case Theme::Dark:
            adw_style_manager_set_color_scheme(adw_style_manager_get_default(), ADW_COLOR_SCHEME_FORCE_DARK);
            break;
        default:
            adw_style_manager_set_color_scheme(adw_style_manager_get_default(), ADW_COLOR_SCHEME_DEFAULT);
            break;
        }
        static Views::MainWindow mainWindow{ m_controller, app };
        mainWindow.show();
    }
}