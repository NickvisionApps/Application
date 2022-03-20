#ifndef WELCOMEPAGE_H
#define WELCOMEPAGE_H

#include <adwaita.h>
#include "../widget.h"

namespace NickvisionApplication::UI::Views
{
    class WelcomePage : public NickvisionApplication::UI::Widget
    {
    public:
        WelcomePage();
        GtkWidget* gobj() override;

    private:
        //==Signals==//
        void openFolder();
    };
}

#endif // WELCOMEPAGE_H
