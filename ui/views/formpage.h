#ifndef FORMPAGE_H
#define FORMPAGE_H

#include <adwaita.h>

namespace NickvisionApplication::UI::Views
{
    typedef void (*Callback_GtkButton)(GtkButton*, gpointer*);
    typedef void (*Callback_GtkDialog_Response)(GtkDialog*, gint, gpointer*);

    class FormPage
    {
    public:
        FormPage();
        GtkWidget* gobj();
        GtkBuilder* getBuilder();
        void show();

    private:
        GtkBuilder* m_builder = nullptr;
        //==Signals==//
        void openFolder();
    };
}

#endif // FORMPAGE_H
