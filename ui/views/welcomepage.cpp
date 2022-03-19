#include "welcomepage.h"
#include "../xmlstrings.h"
#include "../messenger.h"

using namespace NickvisionApplication::UI;
using namespace NickvisionApplication::UI::Views;

WelcomePage::WelcomePage() : m_builder(gtk_builder_new_from_string(XmlStrings::getWelcomePage().c_str(), -1))
{
    //==Signals==//
    g_signal_connect(gtk_builder_get_object(m_builder, "gtk_btnCheckForUpdates"), "clicked", G_CALLBACK((Callback_GtkButton)[](GtkButton* button, gpointer* data) { reinterpret_cast<WelcomePage*>(data)->checkForUpdates(); }), this);
}

GtkWidget* WelcomePage::gobj()
{
    return GTK_WIDGET(gtk_builder_get_object(m_builder, "gtk_boxWelcomePage"));
}

GtkBuilder* WelcomePage::getBuilder()
{
    return m_builder;
}

void WelcomePage::show()
{
    gtk_widget_show(gobj());
}

void WelcomePage::checkForUpdates()
{
    Messenger::getInstance().sendMessage("MainWindow.CheckForUpdates", "");
}
