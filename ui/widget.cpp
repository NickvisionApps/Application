#include "widget.h"

using namespace NickvisionApplication::UI;

Widget::Widget(const std::string& xmlString) : m_builder(gtk_builder_new_from_string(xmlString.c_str(), -1))
{

}

GtkBuilder* Widget::getBuilder()
{
    return m_builder;
}

void Widget::show()
{
    gtk_widget_show(gobj());
}
