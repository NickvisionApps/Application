#ifndef APPLICATION_H
#define APPLICATION_H

#include <string>
#include <memory>
#include <adwaita.h>
#include "views/mainwindow.h"

namespace NickvisionApplication::UI
{
    typedef void (*Callback_GtkApplication)(GtkApplication*, gpointer*);

    class Application
    {
    public:
        Application(const std::string& id, GApplicationFlags flags = G_APPLICATION_FLAGS_NONE);
        AdwApplication* gobj() const;
        int run(int argc, char* argv[]);

    private:
        //==UI==//
        AdwApplication* m_adwApp = nullptr;
        std::shared_ptr<NickvisionApplication::UI::Views::MainWindow> m_mainWindow = nullptr;
        //==Signals==//
        void onActivate(GtkApplication* app);
    };
}

#endif // APPLICATION_H
