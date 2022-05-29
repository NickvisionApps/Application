#pragma once

#include <adwaita.h>
#include "../widget.h"

namespace NickvisionApplication::UI::Views
{
    class ShortcutsDialog : public NickvisionApplication::UI::Widget
    {
    public:
        ShortcutsDialog(GtkWidget* parent);
        ~ShortcutsDialog();

    private:

    };
}
