#include "application.hpp"
#include "../controllers/mainwindowcontroller.hpp"

using namespace NickvisionApplication::Controllers;
using namespace NickvisionApplication::UI;
using namespace NickvisionApplication::UI::Views;

/**
 * Calls Application::onActivate
 *
 * @param app GtkApplication*
 * @param data gpointer* that should point to Application
 */
void callback_activate(GtkApplication* app, gpointer* data) { reinterpret_cast<Application*>(data)->onActivate(app); }

Application::Application(const std::string& id, GApplicationFlags flags) : m_adwApp{ adw_application_new(id.c_str(), flags) }
{
    //AppInfo
    m_appInfo.setName("NickvisionApplication");
    m_appInfo.setShortName("Application");
    m_appInfo.setDescription("A template for creating Nickvision applications.");
    m_appInfo.setVersion("2022.8.0");
    m_appInfo.setChangelog("- Initial Release");
    m_appInfo.setGitHubRepo("https://github.com/nlogozzo/NickvisionApplication");
    m_appInfo.setIssueTracker("https://github.com/nlogozzo/NickvisionApplication/issues/new");
    //Signals
    g_signal_connect(m_adwApp, "activate", G_CALLBACK(callback_activate), this);
}

int Application::run(int argc, char* argv[])
{
    return g_application_run(G_APPLICATION(m_adwApp), argc, argv);
}

void Application::onActivate(GtkApplication* app)
{
    m_mainWindow = std::make_shared<MainWindow>(app, MainWindowController(m_appInfo));
    gtk_application_add_window(app, GTK_WINDOW(m_mainWindow->gobj()));
    m_mainWindow->show();
}
