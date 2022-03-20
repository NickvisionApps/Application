#include "formpage.h"
#include "../xmlstrings.h"
#include "../messenger.h"

using namespace NickvisionApplication::UI;
using namespace NickvisionApplication::UI::Views;

FormPage::FormPage() : Widget(XmlStrings::getFormPage())
{
    //==Signals==//
    g_signal_connect(gtk_builder_get_object(m_builder, "gtk_btnOpenFolder"), "clicked", G_CALLBACK((Callback_GtkButton)[](GtkButton* button, gpointer* data) { reinterpret_cast<FormPage*>(data)->openFolder(); }), this);
    //==Messages==//
    Messenger::getInstance().registerMessage("FormPage.OpenFolder", [&](const std::string& parameter) { openFolder(); });
}

GtkWidget* FormPage::gobj()
{
    return GTK_WIDGET(gtk_builder_get_object(m_builder, "gtk_boxFormPage"));
}

void FormPage::openFolder()
{
    GtkWidget* openFolderDialog = gtk_file_chooser_dialog_new("Open Folder", GTK_WINDOW(gtk_widget_get_root(gobj())), GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER, "_Cancel", GTK_RESPONSE_CANCEL, "_Select", GTK_RESPONSE_ACCEPT, nullptr);
    gtk_window_set_modal(GTK_WINDOW(openFolderDialog), true);
    g_signal_connect(openFolderDialog, "response", G_CALLBACK((Callback_GtkDialog_Response)([](GtkDialog* dialog, gint response_id, gpointer* data)
    {
        if(response_id == GTK_RESPONSE_ACCEPT)
        {
            GtkFileChooser* chooser = GTK_FILE_CHOOSER(dialog);
            GFile* file = gtk_file_chooser_get_file(chooser);
            std::string path(g_file_get_path(file));
            g_object_unref(file);
            Messenger::getInstance().sendMessage("MainWindow.SendToast", "Folder Opened: " + path);
        }
        gtk_window_destroy(GTK_WINDOW(dialog));
    })), nullptr);
    gtk_widget_show(openFolderDialog);
}
