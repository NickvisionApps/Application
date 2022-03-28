#include "widget.h"

using namespace NickvisionApplication::UI;

Widget::Widget(const std::string& resourcePath) : m_builder(gtk_builder_new_from_resource(resourcePath.c_str()))
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
