#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <string>
#include <libnick/aura/configurationbase.h>
#include "theme.h"

namespace Nickvision::Application::Shared::Models
{
    /**
     * @brief A model for the configuration of the application.
     */
    class Configuration : public Nickvision::Aura::ConfigurationBase
    {
    public:
        /**
         * @brief Constructs a Configuration.
         * @param key The key to pass to the ConfigurationBase
         */
        Configuration(const std::string& key) noexcept;
        /**
         * @brief Gets the preferred theme for the application.
         * @return The preferred theme
         */
        Theme getTheme() const noexcept;
        /**
         * @brief Sets the preferred theme for the application.
         * @param theme The new preferred theme
         */
        void setTheme(Theme theme) noexcept;
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

    public:
        /**
         * @brief Gets the configuration object for the running application.
         * @throw std::logic_error Thrown if Aura::init() was not yet called
         * @return The current Configuration instance
         */
        static Configuration& current();
    };
}

#endif //CONFIGURATION_H