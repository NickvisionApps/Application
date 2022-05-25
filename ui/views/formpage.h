#pragma once

#include <adwaita.h>
#include "../widget.h"
#include "../messenger.h"

namespace NickvisionApplication::UI::Views
{
    class FormPage : public NickvisionApplication::UI::Widget
    {
    public:
        FormPage(NickvisionApplication::UI::Messenger& messenger);
        GtkWidget* gobj() override;
        //==Slots==//
        void openFolder();

    private:
        NickvisionApplication::UI::Messenger& m_messenger;
    };
}