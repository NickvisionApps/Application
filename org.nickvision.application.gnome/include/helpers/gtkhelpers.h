#ifndef GTKHELPERS_H
#define GTKHELPERS_H

#include <functional>
#include <adwaita.h>

namespace Nickvision::Application::GNOME::Helpers::GtkHelpers
{
    /**
     * @brief Runs the function on the main UI thread.
     * @param function The function to run 
     */
    void dispatchToMainThread(const std::function<void()>& function);
    /**
     * @brief Sets the accelerator for an action.
     * @param app The GtkApplication
     * @param action The action detailed name
     * @param accel The accelerator
     */
    void setAccelForAction(GtkApplication* app, const char* action, const char* accel);
}

#endif //GTKHELPERS_H