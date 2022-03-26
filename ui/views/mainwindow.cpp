#include "mainwindow.h"
#include <utility>
#include "../../models/configuration.h"
#include "../xmlstrings.h"
#include "../messenger.h"
#include "../controls/progressdialog.h"
#include "preferencesdialog.h"

using namespace NickvisionApplication::Models;
using namespace NickvisionApplication::UI;
using namespace NickvisionApplication::UI::Controls;
using namespace NickvisionApplication::UI::Views;
using namespace NickvisionApplication::Update;

MainWindow::MainWindow() : Widget(XmlStrings::getMainWindow()), m_updater("https://raw.githubusercontent.com/nlogozzo/NickvisionApplication/main/UpdateConfig.json", { "2022.3.0" }), m_opened(false)
{
    //==Help Actions==//
    //Check for Updates
    m_gio_actCheckForUpdates = g_simple_action_new("checkForUpdates", nullptr);
    g_signal_connect(m_gio_actCheckForUpdates, "activate", G_CALLBACK((Callback_GioAction)[](GSimpleAction* action, GVariant* parameter, gpointer* data) { reinterpret_cast<MainWindow*>(data)->checkForUpdates(); }), this);
    g_action_map_add_action(G_ACTION_MAP(MainWindow::gobj()), G_ACTION(m_gio_actCheckForUpdates));
    //GitHub Repo
    m_gio_actGitHubRepo = g_simple_action_new("gitHubRepo", nullptr);
    g_signal_connect(m_gio_actGitHubRepo, "activate", G_CALLBACK((Callback_GioAction)[](GSimpleAction* action, GVariant* parameter, gpointer* data) { reinterpret_cast<MainWindow*>(data)->gitHubRepo(); }), this);
    g_action_map_add_action(G_ACTION_MAP(MainWindow::gobj()), G_ACTION(m_gio_actGitHubRepo));
    //Report a Bug
    m_gio_actReportABug = g_simple_action_new("reportABug", nullptr);
    g_signal_connect(m_gio_actReportABug, "activate", G_CALLBACK((Callback_GioAction)[](GSimpleAction* action, GVariant* parameter, gpointer* data) { reinterpret_cast<MainWindow*>(data)->reportABug(); }), this);
    g_action_map_add_action(G_ACTION_MAP(MainWindow::gobj()), G_ACTION(m_gio_actReportABug));
    //Settings
    m_gio_actPreferences = g_simple_action_new("preferences", nullptr);
    g_signal_connect(m_gio_actPreferences, "activate", G_CALLBACK((Callback_GioAction)[](GSimpleAction* action, GVariant* parameter, gpointer* data) { reinterpret_cast<MainWindow*>(data)->preferences(); }), this);
    g_action_map_add_action(G_ACTION_MAP(MainWindow::gobj()), G_ACTION(m_gio_actPreferences));
    //Changelog
    m_gio_actChangelog = g_simple_action_new("changelog", nullptr);
    g_signal_connect(m_gio_actChangelog, "activate", G_CALLBACK((Callback_GioAction)[](GSimpleAction* action, GVariant* parameter, gpointer* data) { reinterpret_cast<MainWindow*>(data)->changelog(); }), this);
    g_action_map_add_action(G_ACTION_MAP(MainWindow::gobj()), G_ACTION(m_gio_actChangelog));
    //About
    m_gio_actAbout = g_simple_action_new("about", nullptr);
    g_signal_connect(m_gio_actAbout, "activate", G_CALLBACK((Callback_GioAction)[](GSimpleAction* action, GVariant* parameter, gpointer* data) { reinterpret_cast<MainWindow*>(data)->about(); }), this);
    g_action_map_add_action(G_ACTION_MAP(MainWindow::gobj()), G_ACTION(m_gio_actAbout));
    //==Menu Button==//
    GtkBuilder* builderMenu = gtk_builder_new_from_string(XmlStrings::getMenuHelp().c_str(), -1);
    gtk_menu_button_set_menu_model(GTK_MENU_BUTTON(gtk_builder_get_object(m_builder, "gtk_btnHeaderHelp")), G_MENU_MODEL(gtk_builder_get_object(builderMenu, "gio_menuHelp")));
    //==Pages==//
    adw_view_stack_add_named(ADW_VIEW_STACK(gtk_builder_get_object(m_builder, "adw_viewStack")), m_welcomePage.gobj(), "Welcome");
    adw_view_stack_add_named(ADW_VIEW_STACK(gtk_builder_get_object(m_builder, "adw_viewStack")), m_formPage.gobj(), "Form");
    //==Signals==//
    g_signal_connect(MainWindow::gobj(), "show", G_CALLBACK((Callback_GtkWidget)[](GtkWidget* widget, gpointer* data) { reinterpret_cast<MainWindow*>(data)->onStartup(); }), this);
    g_signal_connect(gtk_builder_get_object(m_builder, "gtk_listNavigation"), "row-selected", G_CALLBACK((Callback_GtkListBox_Selection)[](GtkListBox* listBox, GtkListBoxRow* row, gpointer* data) { reinterpret_cast<MainWindow*>(data)->onNavigationChanged(); }), this);
    //==Messages==//
    Messenger::getInstance().registerMessage("MainWindow.SendToast", [&](const std::string& parameter) { sendToast(parameter); });
    Messenger::getInstance().registerMessage("MainWindow.ChangePage", [&](const std::string& parameter) { changePage(parameter); });
    Messenger::getInstance().registerMessage("MainWindow.Preferences", [&](const std::string& parameter) { preferences(); });
    //==Navigation==//
    gtk_list_box_select_row(GTK_LIST_BOX(gtk_builder_get_object(m_builder, "gtk_listNavigation")), gtk_list_box_get_row_at_index(GTK_LIST_BOX(gtk_builder_get_object(m_builder, "gtk_listNavigation")), 0));
}

MainWindow::~MainWindow()
{
    Configuration configuration;
    configuration.save();
    gtk_window_destroy(GTK_WINDOW(MainWindow::gobj()));
}

GtkWidget* MainWindow::gobj()
{
    return GTK_WIDGET(gtk_builder_get_object(m_builder, "adw_winMain"));
}

void MainWindow::showMaximized()
{
    gtk_widget_show(gobj());
    gtk_window_maximize(GTK_WINDOW(gobj()));
}

void MainWindow::onStartup()
{
    if(!m_opened)
    {
        Configuration configuration;
        configuration.setIsFirstTimeOpen(false);
        configuration.save();
        m_opened = true;
    }
}

void MainWindow::checkForUpdates()
{
    ProgressDialog* checkingDialog = new ProgressDialog(gobj(), "Checking for updates...", [&]() { m_updater.checkForUpdates(); });
    std::pair<ProgressDialog*, MainWindow*>* pointers = new std::pair<ProgressDialog*, MainWindow*>(checkingDialog, this);
    g_signal_connect(checkingDialog->gobj(), "hide", G_CALLBACK((Callback_GtkWidget)([](GtkWidget* widget, gpointer* data)
    {
        std::pair<ProgressDialog*, MainWindow*>* pointers = reinterpret_cast<std::pair<ProgressDialog*, MainWindow*>*>(data);
        delete pointers->first;
        if(pointers->second->m_updater.getUpdateAvailable())
        {
            GtkWidget* updateDialog = gtk_message_dialog_new(GTK_WINDOW(pointers->second->gobj()), GtkDialogFlags(GTK_DIALOG_DESTROY_WITH_PARENT | GTK_DIALOG_MODAL),
                                                                GTK_MESSAGE_INFO, GTK_BUTTONS_YES_NO, "Update Available");
            gtk_message_dialog_format_secondary_text(GTK_MESSAGE_DIALOG(updateDialog), std::string("\n===V" + pointers->second->m_updater.getLatestVersion()->toString() + " Changelog===\n" + pointers->second->m_updater.getChangelog() + "\n\nNickvisionApplication can automatically download the latest executable to your Downloads directory. Would you like to continue?").c_str());
            g_signal_connect(updateDialog, "response", G_CALLBACK((Callback_GtkDialog_Response)([](GtkDialog* dialog, gint response_id, gpointer* data)
            {
                MainWindow* mainWindow = reinterpret_cast<MainWindow*>(data);
                gtk_window_destroy(GTK_WINDOW(dialog));
                if(response_id == GTK_RESPONSE_YES)
                {
                    ProgressDialog* downloadingDialog = new ProgressDialog(mainWindow->gobj(), "Downloading the update...", [&]() { mainWindow->m_updater.update(); });
                    std::pair<ProgressDialog*, MainWindow*>* pointers = new std::pair<ProgressDialog*, MainWindow*>(downloadingDialog, mainWindow);
                    g_signal_connect(downloadingDialog->gobj(), "destroy", G_CALLBACK((Callback_GtkWidget)([](GtkWidget* widget, gpointer* data)
                    {
                        std::pair<ProgressDialog*, MainWindow*>* pointers = reinterpret_cast<std::pair<ProgressDialog*, MainWindow*>*>(data);
                        delete pointers->first;
                        if(pointers->second->m_updater.getUpdateSuccessful())
                        {
                            GtkWidget* successDialog = gtk_message_dialog_new(GTK_WINDOW(pointers->second->gobj()), GtkDialogFlags(GTK_DIALOG_DESTROY_WITH_PARENT | GTK_DIALOG_MODAL),
                                                                           GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "Update Downloaded Successfully");
                            gtk_message_dialog_format_secondary_text(GTK_MESSAGE_DIALOG(successDialog), "We recommend moving the new executable version out of your Downloads directory and running it from elsewhere to allow future updates to download smoothly.");
                            g_signal_connect(successDialog, "response", G_CALLBACK(gtk_window_destroy), nullptr);
                            gtk_widget_show(successDialog);
                        }
                        else
                        {
                            pointers->second->sendToast("Error: Unable to download the update.");
                        }
                    })), pointers);
                    downloadingDialog->show();
                }
            })), pointers->second);
            gtk_widget_show(updateDialog);
        }
        else
        {
           pointers->second->sendToast("There is no update at this time. Please check again later.");
        }
        delete pointers;
    })), pointers);
    checkingDialog->show();
}

void MainWindow::gitHubRepo()
{
    g_app_info_launch_default_for_uri("https://github.com/nlogozzo/NickvisionApplication", nullptr, nullptr);
}

void MainWindow::reportABug()
{
    g_app_info_launch_default_for_uri("https://github.com/nlogozzo/NickvisionApplication/issues/new", nullptr, nullptr);
}

void MainWindow::preferences()
{
    PreferencesDialog* preferencesDialog = new PreferencesDialog(gobj());
    std::pair<PreferencesDialog*, MainWindow*>* pointers = new std::pair<PreferencesDialog*, MainWindow*>(preferencesDialog, this);
    g_signal_connect(preferencesDialog->gobj(), "hide", G_CALLBACK((Callback_GtkWidget)([](GtkWidget* widget, gpointer* data)
    {
        std::pair<PreferencesDialog*, MainWindow*>* pointers = reinterpret_cast<std::pair<PreferencesDialog*, MainWindow*>*>(data);
        delete pointers->first;
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
        delete pointers;
    })), pointers);
    preferencesDialog->show();
}

void MainWindow::changelog()
{
    GtkWidget* changelogDialog = gtk_message_dialog_new(GTK_WINDOW(gobj()), GtkDialogFlags(GTK_DIALOG_DESTROY_WITH_PARENT | GTK_DIALOG_MODAL),
                                                        GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "What's New?");
    gtk_message_dialog_format_secondary_text(GTK_MESSAGE_DIALOG(changelogDialog), "- Initial Release");
    g_signal_connect(changelogDialog, "response", G_CALLBACK(gtk_window_destroy), nullptr);
    gtk_widget_show(changelogDialog);
}

void MainWindow::about()
{
    const char* authors[] = { "Nicholas Logozzo", nullptr };
    gtk_show_about_dialog(GTK_WINDOW(gobj()), "program-name", "NickvisionApplication", "version", "2022.3.0", "comments", "A template for creating Nickvision applications.",
                          "copyright", "(C) Nickvision 2021-2022", "license-type", GTK_LICENSE_GPL_3_0, "website", "https://github.com/nlogozzo", "website-label", "GitHub",
                          "authors", authors, nullptr);
}

void MainWindow::onNavigationChanged()
{
    GtkListBoxRow* selectedRow = gtk_list_box_get_selected_row(GTK_LIST_BOX(gtk_builder_get_object(m_builder, "gtk_listNavigation")));
    int selectedIndex = gtk_list_box_row_get_index(selectedRow);
    if(selectedIndex == 0)
    {
        adw_view_stack_set_visible_child_name(ADW_VIEW_STACK(gtk_builder_get_object(m_builder, "adw_viewStack")), "Welcome");
    }
    else if(selectedIndex == 1)
    {
        adw_view_stack_set_visible_child_name(ADW_VIEW_STACK(gtk_builder_get_object(m_builder, "adw_viewStack")), "Form");
    }
}

void MainWindow::sendToast(const std::string& message)
{
    AdwToast* toast = adw_toast_new(message.c_str());
    adw_toast_overlay_add_toast(ADW_TOAST_OVERLAY(gtk_builder_get_object(m_builder, "adw_toastOverlay")), toast);
}

void MainWindow::changePage(const std::string& pageName)
{
    if(pageName == "Welcome")
    {
        gtk_list_box_select_row(GTK_LIST_BOX(gtk_builder_get_object(m_builder, "gtk_listNavigation")), gtk_list_box_get_row_at_index(GTK_LIST_BOX(gtk_builder_get_object(m_builder, "gtk_listNavigation")), 0));
    }
    else if(pageName == "Form")
    {
        gtk_list_box_select_row(GTK_LIST_BOX(gtk_builder_get_object(m_builder, "gtk_listNavigation")), gtk_list_box_get_row_at_index(GTK_LIST_BOX(gtk_builder_get_object(m_builder, "gtk_listNavigation")), 1));
    }
    else
    {
        gtk_list_box_select_row(GTK_LIST_BOX(gtk_builder_get_object(m_builder, "gtk_listNavigation")), gtk_list_box_get_row_at_index(GTK_LIST_BOX(gtk_builder_get_object(m_builder, "gtk_listNavigation")), 0));
    }
}
