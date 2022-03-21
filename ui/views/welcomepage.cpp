#include "welcomepage.h"
#include "../xmlstrings.h"
#include "../messenger.h"

using namespace NickvisionApplication::UI;
using namespace NickvisionApplication::UI::Views;

WelcomePage::WelcomePage() : Widget(XmlStrings::getWelcomePage())
{
    //==Signals==//
    g_signal_connect(gtk_builder_get_object(m_builder, "gtk_btnPreferences"), "clicked", G_CALLBACK((Callback_GtkButton)[](GtkButton* button, gpointer* data) { reinterpret_cast<WelcomePage*>(data)->preferences(); }), this);
    g_signal_connect(gtk_builder_get_object(m_builder, "gtk_btnOpenFolder"), "clicked", G_CALLBACK((Callback_GtkButton)[](GtkButton* button, gpointer* data) { reinterpret_cast<WelcomePage*>(data)->openFolder(); }), this);
}

GtkWidget* WelcomePage::gobj()
{
    return GTK_WIDGET(gtk_builder_get_object(m_builder, "gtk_boxWelcomePage"));
}

void WelcomePage::preferences()
{
    Messenger::getInstance().sendMessage("MainWindow.Preferences", "");
}

void WelcomePage::openFolder()
{
    Messenger::getInstance().sendMessage("FormPage.OpenFolder", "");
    Messenger::getInstance().sendMessage("MainWindow.ChangePage", "Form");
}
