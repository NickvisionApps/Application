#include "formpage.h"

using namespace NickvisionApplication::UI::Views;

FormPage::FormPage() : Widget("/ui/views/formpage.xml")
{

}

GtkWidget* FormPage::gobj()
{
    return GTK_WIDGET(gtk_builder_get_object(m_builder, "adw_formPage"));
}