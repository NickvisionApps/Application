#pragma once

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
        
    };
}