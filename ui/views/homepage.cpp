#include "homepage.h"

using namespace NickvisionApplication::UI::Views;

HomePage::HomePage() : Widget{"/ui/views/homepage.xml"}
{

}

GtkWidget* HomePage::gobj()
{
    return GTK_WIDGET(gtk_builder_get_object(m_builder, "adw_homePage"));
}