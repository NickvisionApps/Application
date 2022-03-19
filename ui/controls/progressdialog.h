#ifndef PROGRESSDIALOG_H
#define PROGRESSDIALOG_H

#include <string>
#include <thread>
#include <functional>
#include <adwaita.h>

namespace NickvisionApplication::UI::Controls
{
    class ProgressDialog
    {
    public:
        ProgressDialog(GtkWidget* parent, const std::string& description, const std::function<void()>& work);
        ~ProgressDialog();
        GtkWidget* gobj();
        GtkBuilder* getBuilder();
        void show();

    private:
        GtkBuilder* m_builder = nullptr;
        std::function<void()> m_work;
        bool m_isFinished;
        std::jthread m_thread;
        //==Singals==//
        bool timeout();
    };
}

#endif // PROGRESSDIALOG_H
