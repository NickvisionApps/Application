#pragma once

#include <adwaita.h>
#include "../widget.h"

namespace NickvisionApplication::UI::Views
{
    class HomePage : public NickvisionApplication::UI::Widget
    {
    public:
        HomePage();
        GtkWidget* gobj() override;

    private:
        
    };
}