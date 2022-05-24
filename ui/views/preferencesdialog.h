#pragma once

#include <adwaita.h>
#include "../widget.h"
#include "../../models/configuration.h"

namespace NickvisionApplication::UI::Views
{
    class PreferencesDialog : public NickvisionApplication::UI::Widget
    {
    public:
        PreferencesDialog(GtkWidget* parent, NickvisionApplication::Models::Configuration& configuration);
        ~PreferencesDialog();
        GtkWidget* gobj() override;

    private:
        NickvisionApplication::Models::Configuration& m_configuration;
        //==Signals==//
        void cancel();
        void save();
        void onRowIsFirstTimeOpenActivate();
    };
}
