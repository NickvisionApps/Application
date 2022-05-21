#ifndef PREFERENCESDIALOG_H
#define PREFERENCESDIALOG_H

#include <adwaita.h>
#include "../widget.h"
#include "../../models/configuration.h"

namespace NickvisionApplication::UI::Views
{
    class PreferencesDialog : public NickvisionApplication::UI::Widget
    {
    public:
        PreferencesDialog(GtkWidget* parent);
        ~PreferencesDialog();
        GtkWidget* gobj() override;

    private:
        NickvisionApplication::Models::Configuration m_configuration;
        //==Signals==//
        void save();
        void onRowIsFirstTimeOpenActivate();
    };
}

#endif // PREFERENCESDIALOG_H
