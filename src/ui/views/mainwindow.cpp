#include "mainwindow.hpp"
#include <utility>
#include "preferencesdialog.hpp"
#include "../controls/progressdialog.hpp"

using namespace NickvisionApplication::Controllers;
using namespace NickvisionApplication::UI::Controls;
using namespace NickvisionApplication::UI::Views;

MainWindow::MainWindow(GtkApplication* application, const MainWindowController& controller) : m_controller{ controller }, m_gobj{ adw_application_window_new(application) }
{
    //Window Settings
    gtk_window_set_default_size(GTK_WINDOW(m_gobj), 1000, 800);
    g_signal_connect(m_gobj, "show", G_CALLBACK((void (*)(GtkWidget*, gpointer*))[](GtkWidget*, gpointer* data) { reinterpret_cast<MainWindow*>(data)->onStartup(); }), this);
    //Header Bar
    m_headerBar = adw_header_bar_new();
    m_adwTitle = adw_window_title_new(m_controller.getAppInfo().getShortName().c_str(), m_controller.getFolderPath().c_str());
    adw_header_bar_set_title_widget(ADW_HEADER_BAR(m_headerBar), m_adwTitle);
    //Open Folder Button
    m_btnOpenFolder = gtk_button_new();
    GtkWidget* btnOpenFolderContent{ adw_button_content_new() };
    adw_button_content_set_icon_name(ADW_BUTTON_CONTENT(btnOpenFolderContent), "folder-open-symbolic");
    adw_button_content_set_label(ADW_BUTTON_CONTENT(btnOpenFolderContent), "Open");
    gtk_button_set_child(GTK_BUTTON(m_btnOpenFolder), btnOpenFolderContent);
    gtk_widget_set_tooltip_text(m_btnOpenFolder, "Open Folder (Ctrl+O)");
    gtk_actionable_set_action_name(GTK_ACTIONABLE(m_btnOpenFolder), "win.openFolder");
    adw_header_bar_pack_start(ADW_HEADER_BAR(m_headerBar), m_btnOpenFolder);
    //Close Folder Button
    m_btnCloseFolder = gtk_button_new();
    gtk_button_set_icon_name(GTK_BUTTON(m_btnCloseFolder), "window-close-symbolic");
    gtk_widget_set_tooltip_text(m_btnCloseFolder, "Close Folder (Ctrl+W)");
    gtk_widget_set_visible(m_btnCloseFolder, false);
    gtk_actionable_set_action_name(GTK_ACTIONABLE(m_btnCloseFolder), "win.closeFolder");
    adw_header_bar_pack_start(ADW_HEADER_BAR(m_headerBar), m_btnCloseFolder);
    //Menu Help Button
    m_btnMenuHelp = gtk_menu_button_new();
    GMenu* menuHelp{ g_menu_new() };
    g_menu_append(menuHelp, "Preferences", "win.preferences");
    g_menu_append(menuHelp, "Changelog", "win.changelog");
    g_menu_append(menuHelp, std::string("About " + m_controller.getAppInfo().getShortName()).c_str(), "win.about");
    gtk_menu_button_set_direction(GTK_MENU_BUTTON(m_btnMenuHelp), GTK_ARROW_NONE);
    gtk_menu_button_set_menu_model(GTK_MENU_BUTTON(m_btnMenuHelp), G_MENU_MODEL(menuHelp));
    gtk_widget_set_tooltip_text(m_btnMenuHelp, "Main Menu");
    adw_header_bar_pack_end(ADW_HEADER_BAR(m_headerBar), m_btnMenuHelp);
    //Toast Overlay
    m_toastOverlay = adw_toast_overlay_new();
    gtk_widget_set_hexpand(m_toastOverlay, true);
    gtk_widget_set_vexpand(m_toastOverlay, true);
    //Main Box
    m_mainBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_append(GTK_BOX(m_mainBox), m_headerBar);
    gtk_box_append(GTK_BOX(m_mainBox), m_toastOverlay);
    adw_application_window_set_content(ADW_APPLICATION_WINDOW(m_gobj), m_mainBox);
    //Send Toast Callback
    m_controller.registerSendToastCallback([&](const std::string& message) { adw_toast_overlay_add_toast(ADW_TOAST_OVERLAY(m_toastOverlay), adw_toast_new(message.c_str())); });
    //Folder Changed Callback
    m_controller.registerFolderChangedCallback([&]() { onFolderChanged(); });
    //Open Folder Action
    m_actOpenFolder = g_simple_action_new("openFolder", nullptr);
    g_signal_connect(m_actOpenFolder, "activate", G_CALLBACK((void (*)(GSimpleAction*, GVariant*, gpointer*))[](GSimpleAction*, GVariant*, gpointer* data) { reinterpret_cast<MainWindow*>(data)->onOpenFolder(); }), this);
    g_action_map_add_action(G_ACTION_MAP(m_gobj), G_ACTION(m_actOpenFolder));
    gtk_application_set_accels_for_action(application, "win.openFolder", new const char*[2]{ "<Ctrl>o", nullptr });
    //Close Folder Action
    m_actCloseFolder = g_simple_action_new("closeFolder", nullptr);
    g_signal_connect(m_actCloseFolder, "activate", G_CALLBACK((void (*)(GSimpleAction*, GVariant*, gpointer*))[](GSimpleAction*, GVariant*, gpointer* data) { reinterpret_cast<MainWindow*>(data)->m_controller.closeFolder(); }), this);
    g_action_map_add_action(G_ACTION_MAP(m_gobj), G_ACTION(m_actCloseFolder));
    gtk_application_set_accels_for_action(application, "win.closeFolder", new const char*[2]{ "<Ctrl>w", nullptr });
    //Preferences Action
    m_actPreferences = g_simple_action_new("preferences", nullptr);
    g_signal_connect(m_actPreferences, "activate", G_CALLBACK((void (*)(GSimpleAction*, GVariant*, gpointer*))[](GSimpleAction*, GVariant*, gpointer* data) { reinterpret_cast<MainWindow*>(data)->onPreferences(); }), this);
    g_action_map_add_action(G_ACTION_MAP(m_gobj), G_ACTION(m_actPreferences));
    gtk_application_set_accels_for_action(application, "win.preferences", new const char*[2]{ "<Ctrl>period", nullptr });
    //Changelog Action
    m_actChangelog = g_simple_action_new("changelog", nullptr);
    g_signal_connect(m_actChangelog, "activate", G_CALLBACK((void (*)(GSimpleAction*, GVariant*, gpointer*))[](GSimpleAction*, GVariant*, gpointer* data) { reinterpret_cast<MainWindow*>(data)->onChangelog(); }), this);
    g_action_map_add_action(G_ACTION_MAP(m_gobj), G_ACTION(m_actChangelog));
    //About Action
    m_actAbout = g_simple_action_new("about", nullptr);
    g_signal_connect(m_actAbout, "activate", G_CALLBACK((void (*)(GSimpleAction*, GVariant*, gpointer*))[](GSimpleAction*, GVariant*, gpointer* data) { reinterpret_cast<MainWindow*>(data)->onAbout(); }), this);
    g_action_map_add_action(G_ACTION_MAP(m_gobj), G_ACTION(m_actAbout));
    gtk_application_set_accels_for_action(application, "win.about", new const char*[2]{ "F1", nullptr });
}

GtkWidget* MainWindow::gobj()
{
    return m_gobj;
}

void MainWindow::show()
{
    gtk_widget_show(m_gobj);
}

void MainWindow::onStartup()
{
    ProgressDialog* progressDialog{ new ProgressDialog(GTK_WINDOW(m_gobj), "Starting application...", [&]()
    {
        m_controller.startup();
    }, []() {}) };
    progressDialog->show();
}

void MainWindow::onFolderChanged()
{
    adw_window_title_set_subtitle(ADW_WINDOW_TITLE(m_adwTitle), m_controller.getFolderPath().c_str());
    gtk_widget_set_visible(m_btnCloseFolder, m_controller.getIsFolderValid());
}

void MainWindow::onOpenFolder()
{
    GtkFileChooserNative* openFolderDialog{ gtk_file_chooser_native_new("Open Folder", GTK_WINDOW(m_gobj), GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER, "_Open", "_Cancel") };
    gtk_native_dialog_set_modal(GTK_NATIVE_DIALOG(openFolderDialog), true);
    g_signal_connect(openFolderDialog, "response", G_CALLBACK((void (*)(GtkNativeDialog*, gint, gpointer*))([](GtkNativeDialog* dialog, gint response_id, gpointer* data)
    {
        if(response_id == GTK_RESPONSE_ACCEPT)
        {
            MainWindow* mainWindow{ reinterpret_cast<MainWindow*>(data) };
            GFile* file{ gtk_file_chooser_get_file(GTK_FILE_CHOOSER(dialog)) };
            mainWindow->m_controller.openFolder(g_file_get_path(file));
            g_object_unref(file);
        }
        g_object_unref(dialog);
    })), this);
    gtk_native_dialog_show(GTK_NATIVE_DIALOG(openFolderDialog));
}

void MainWindow::onPreferences()
{
    PreferencesDialog* preferencesDialog{ new PreferencesDialog(GTK_WINDOW(m_gobj), m_controller.createPreferencesDialogController()) };
    std::pair<PreferencesDialog*, MainWindow*>* pointers{ new std::pair<PreferencesDialog*, MainWindow*>(preferencesDialog, this) };
    g_signal_connect(preferencesDialog->gobj(), "hide", G_CALLBACK((void (*)(GtkWidget*, gpointer*))([](GtkWidget*, gpointer* data)
    {
        std::pair<PreferencesDialog*, MainWindow*>* pointers{reinterpret_cast<std::pair<PreferencesDialog*, MainWindow*>*>(data)};
        delete pointers->first;
        delete pointers;
    })), pointers);
    preferencesDialog->show();
}

void MainWindow::onChangelog()
{
    GtkWidget* changelogDialog{ gtk_message_dialog_new(GTK_WINDOW(m_gobj), GtkDialogFlags(GTK_DIALOG_MODAL), GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "What's New?") };
    gtk_message_dialog_format_secondary_text(GTK_MESSAGE_DIALOG(changelogDialog), m_controller.getAppInfo().getChangelog().c_str());
    g_signal_connect(changelogDialog, "response", G_CALLBACK(gtk_window_destroy), nullptr);
    gtk_widget_show(changelogDialog);
}

void MainWindow::onAbout()
{
    gtk_show_about_dialog(GTK_WINDOW(m_gobj), "program-name", m_controller.getAppInfo().getName().c_str(), "version", m_controller.getAppInfo().getVersion().c_str(),
                          "comments", m_controller.getAppInfo().getDescription().c_str(),
                          "copyright", "(C) Nickvision 2021-2022", "license-type", GTK_LICENSE_GPL_3_0, "website", m_controller.getAppInfo().getGitHubRepo().c_str(),
                          "website-label", "GitHub", "authors", new const char*[2]{ "Nicholas Logozzo", nullptr }, nullptr);
    /*
    adw_show_about_window(GTK_WINDOW(m_gobj),
                          "application-name", m_controller.getAppInfo().getShortName().c_str(),
                          "application-icon", "org.nickvision.application",
                          "version", m_controller.getAppInfo().getVersion().c_str(),
                          "comments", m_controller.getAppInfo().getDescription().c_str(),
                          "developer-name", "Nickvision",
                          "license-type", GTK_LICENSE_GPL_3_0,
                          "copyright", "(C) Nickvision 2021-2022",
                          "issue-url", m_controller.getAppInfo().getIssueTracker().c_str(),
                          "website", m_controller.getAppInfo().getGitHubRepo().c_str(),
                          "developers", new const char*[2]{ "Nicholas Logozzo", nullptr },
                          "release-notes", m_controller.getAppInfo().getChangelog().c_str(),
                          nullptr);
    */
}
