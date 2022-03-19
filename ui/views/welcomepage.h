#ifndef WELCOMEPAGE_H
#define WELCOMEPAGE_H

#include <adwaita.h>

namespace NickvisionApplication::UI::Views
{
    typedef void (*Callback_GtkButton)(GtkButton*, gpointer*);

    class WelcomePage
    {
    public:
        WelcomePage();
        GtkWidget* gobj();
        GtkBuilder* getBuilder();
        void show();

    private:
        GtkBuilder* m_builder = nullptr;
        //==Signals==//
        void checkForUpdates();
    };
}

#endif // WELCOMEPAGE_H
