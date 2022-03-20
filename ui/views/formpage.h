#ifndef FORMPAGE_H
#define FORMPAGE_H

#include <adwaita.h>
#include "../widget.h"

namespace NickvisionApplication::UI::Views
{
    class FormPage : public NickvisionApplication::UI::Widget
    {
    public:
        FormPage();
        GtkWidget* gobj() override;

    private:
        //==Signals==//
        void openFolder();
    };
}

#endif // FORMPAGE_H
