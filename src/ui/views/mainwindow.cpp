#include "mainwindow.hpp"

using namespace NickvisionApplication::Controllers;
using namespace NickvisionApplication::UI::Views;

MainWindow::MainWindow(GtkApplication* application, const MainWindowController& controller) : m_controller{ controller }, m_gobj{ adw_application_window_new(application) }
{
    //Window Settings
    gtk_window_set_default_size(GTK_WINDOW(m_gobj), 800, 600);
    //Header Bar
    m_headerBar = adw_header_bar_new();
    m_adwTitle = adw_window_title_new(m_controller.getApplicationShortName().c_str(), m_controller.getFolderPath().c_str());
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
    //Toast Overlay
    m_toastOverlay = adw_toast_overlay_new();
    gtk_widget_set_hexpand(m_toastOverlay, true);
    gtk_widget_set_vexpand(m_toastOverlay, true);
    //Main Box
    m_mainBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_append(GTK_BOX(m_mainBox), m_headerBar);
    gtk_box_append(GTK_BOX(m_mainBox), m_toastOverlay);
    adw_application_window_set_content(ADW_APPLICATION_WINDOW(m_gobj), m_mainBox);
    //Open Folder Action
    m_actOpenFolder = g_simple_action_new("openFolder", nullptr);
    g_signal_connect(m_actOpenFolder, "activate", G_CALLBACK((void (*)(GSimpleAction*, GVariant*, gpointer*))[](GSimpleAction*, GVariant*, gpointer* data) { reinterpret_cast<MainWindow*>(data)->onOpenFolder(); }), this);
    g_action_map_add_action(G_ACTION_MAP(m_gobj), G_ACTION(m_actOpenFolder));
    gtk_application_set_accels_for_action(application, "win.openFolder", new const char*[2]{ "<Ctrl>o", nullptr });
    //Close Folder Action
    m_actCloseFolder = g_simple_action_new("closeFolder", nullptr);
    g_signal_connect(m_actCloseFolder, "activate", G_CALLBACK((void (*)(GSimpleAction*, GVariant*, gpointer*))[](GSimpleAction*, GVariant*, gpointer* data) { reinterpret_cast<MainWindow*>(data)->onCloseFolder(); }), this);
    g_action_map_add_action(G_ACTION_MAP(m_gobj), G_ACTION(m_actCloseFolder));
    gtk_application_set_accels_for_action(application, "win.closeFolder", new const char*[2]{ "<Ctrl>w", nullptr });
    //Folder Changed Event
    m_controller.registerFolderChangedCallback([&]() { onFolderChanged(); });
}

GtkWidget* MainWindow::gobj() const
{
    return m_gobj;
}

void MainWindow::show()
{
    gtk_widget_show(m_gobj);
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

void MainWindow::onCloseFolder()
{
    m_controller.closeFolder();
}
