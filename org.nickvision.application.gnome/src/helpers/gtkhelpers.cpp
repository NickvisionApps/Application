#include "helpers/gtkhelpers.h"
#include <adwaita.h>

namespace Nickvision::Application::GNOME::Helpers
{
    void GtkHelpers::dispatchToMainThread(const std::function<void()>& func)
    {
        g_main_context_invoke(g_main_context_default(), +[](gpointer data) -> int
        {
            std::function<void()>* function{ static_cast<std::function<void()>*>(data) };
            (*function)();
            delete function;
            return false;
        }, new std::function<void()>(func));
    }

    void GtkHelpers::setAccelForAction(GtkApplication* app, const char* action, const char* accel)
    {
        const char* accels[2] { accel, nullptr };
        gtk_application_set_accels_for_action(app, action, accels);
    }
}