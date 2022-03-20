#ifndef PROGRESSDIALOG_H
#define PROGRESSDIALOG_H

#include <string>
#include <thread>
#include <functional>
#include <adwaita.h>
#include "../widget.h"

namespace NickvisionApplication::UI::Controls
{
    class ProgressDialog : public NickvisionApplication::UI::Widget
    {
    public:
        ProgressDialog(GtkWidget* parent, const std::string& description, const std::function<void()>& work);
        ~ProgressDialog();
        GtkWidget* gobj() override;
        void show() override;

    private:
        std::function<void()> m_work;
        bool m_isFinished;
        std::jthread m_thread;
        //==Singals==//
        bool timeout();
    };
}

#endif // PROGRESSDIALOG_H
