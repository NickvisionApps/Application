#ifndef PREFERENCESVIEWCONTROLLER_H
#define PREFERENCESVIEWCONTROLLER_H

#include <string>
#include "models/theme.h"

namespace Nickvision::Application::Shared::Controllers
{
    /**
     * @brief A controller for a PreferencesView.
     */
    class PreferencesViewController
    {
    public:
        /**
         * @brief Constructs a PreferencesViewController.
         */
        PreferencesViewController() noexcept = default;
        /**
         * @brief Gets the application's id.
         * @return The app id
         */
        const std::string& getId() const noexcept;
        /**
         * @brief Gets the preferred theme for the application.
         * @return The preferred theme
         */
        Models::Theme getTheme() const noexcept;
        /**
         * @brief Sets the preferred theme for the application.
         * @param theme The new preferred theme
         */
        void setTheme(Models::Theme theme) noexcept;
        /**
         * @brief Gets whether or not to automatically check for application updates.
         * @return True to automatically check for updates, else false
         */
        bool getAutomaticallyCheckForUpdates() const noexcept;
        /**
         * @brief Sets whether or not to automatically check for application updates.
         * @param check Whether or not to automatically check for updates
         */
        void setAutomaticallyCheckForUpdates(bool check) noexcept;
        /**
         * @brief Saves the current configuration to disk.
         */
        void saveConfiguration() noexcept;
    };
}

#endif //PREFERENCESVIEWCONTROLLER_H