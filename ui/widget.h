#ifndef WIDGET_H
#define WIDGET_H

#include <string>
#include <adwaita.h>

namespace NickvisionApplication::UI
{
    typedef void (*Callback_GioAction)(GSimpleAction*, GVariant*, gpointer*);
    typedef void (*Callback_GtkWidget)(GtkWidget*, gpointer*);
    typedef void (*Callback_GtkDialog_Response)(GtkDialog*, gint, gpointer*);
    typedef void (*Callback_GtkButton)(GtkButton*, gpointer*);
    typedef void (*Callback_GtkListBox_Selection)(GtkListBox*, GtkListBoxRow*, gpointer*);

    class Widget
    {
    public:
        Widget(const std::string& xmlString);
        GtkBuilder* getBuilder();
        virtual GtkWidget* gobj() = 0;
        virtual void show();

    protected:
        GtkBuilder* m_builder = nullptr;
    };
}

#endif // WIDGET_H
