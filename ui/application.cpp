#include "application.h"
#include "../models/configuration.h"

using namespace NickvisionApplication::Models;
using namespace NickvisionApplication::UI;
using namespace NickvisionApplication::UI::Views;

Application::Application(const std::string& id, GApplicationFlags flags) : m_adwApp(adw_application_new(id.c_str(), flags))
{
    g_signal_connect(m_adwApp, "activate", G_CALLBACK((Callback_GtkApplication)[](GtkApplication* app, gpointer* data) { reinterpret_cast<Application*>(data)->onActivate(app); }), this);
}

AdwApplication* Application::gobj() const
{
    return m_adwApp;
}

int Application::run(int argc, char* argv[])
{
    return g_application_run(G_APPLICATION(m_adwApp), argc, argv);
}

void Application::onActivate(GtkApplication* app)
{
    //==Configuration==//
    Configuration configuration;
    if(configuration.getTheme() == Theme::System)
    {
        adw_style_manager_set_color_scheme(adw_style_manager_get_default(), ADW_COLOR_SCHEME_PREFER_LIGHT);
    }
    else if(configuration.getTheme() == Theme::Light)
    {
        adw_style_manager_set_color_scheme(adw_style_manager_get_default(), ADW_COLOR_SCHEME_FORCE_LIGHT);
    }
    else if(configuration.getTheme() == Theme::Dark)
    {
        adw_style_manager_set_color_scheme(adw_style_manager_get_default(), ADW_COLOR_SCHEME_FORCE_DARK);
    }
    configuration.setIsFirstTimeOpen(false);
    configuration.save();
    //==Main Window==//
    m_mainWindow = std::make_shared<MainWindow>();
    gtk_application_add_window(app, GTK_WINDOW(m_mainWindow->gobj()));
    m_mainWindow->showMaximized();
}
