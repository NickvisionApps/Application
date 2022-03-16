#ifndef PROGRESSDIALOG_H
#define PROGRESSDIALOG_H

#include <string>
#include <thread>
#include <functional>
#include <adwaita.h>

namespace NickvisionApplication::Controls
{
    class ProgressDialog
    {
    public:
        ProgressDialog(GtkWidget* parent, const std::string& description, const std::function<void()>& work);
        ~ProgressDialog();
        GtkWidget* gobj() const;
        void show();

    private:
        std::function<void()> m_work;
        bool m_isFinished;
        std::jthread m_thread;
        GtkBuilder* m_builder = nullptr;
        //==Singals==//
        bool timeout();
    };
}

#endif // PROGRESSDIALOG_H
