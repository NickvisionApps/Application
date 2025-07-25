#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <string>
#include <libnick/app/datafilebase.h>
#include <libnick/app/windowgeometry.h>
#include "theme.h"

namespace Nickvision::Application::Shared::Models
{
    /**
     * @brief A model for the configuration of the application.
     */
    class Configuration : public Nickvision::App::DataFileBase
    {
    public:
        /**
         * @brief Constructs a Configuration.
         * @param key The key to pass to the DataFileBase
         * @param appName The appName to pass to the DataFileBase
         * @param isPortable The isPortable to pass to the DataFileBase
         */
        Configuration(const std::string& key, const std::string& appName, bool isPortable);
        /**
         * @brief Gets the preferred theme for the application.
         * @return The preferred theme
         */
        Theme getTheme() const;
        /**
         * @brief Sets the preferred theme for the application.
         * @param theme The new preferred theme
         */
        void setTheme(Theme theme);
        /**
         * @brief Gets the preferred translation language for the application.
         * @return The preferred translation language
         * @return An empty string to use the system language
         * @return "C" to not use translations
         */
        std::string getTranslationLanguage() const;
        /**
         * @brief Sets the preferred translation language for the application.
         * @param language The new preferred translation language
         */
        void setTranslationLanguage(const std::string& language);
        /**
         * @brief Gets the window geometry for the application.
         * @return The window geometry
         */
        App::WindowGeometry getWindowGeometry() const;
        /**
         * @brief Sets the window geometry for the application.
         * @param geometry The new window geometry
         */
        void setWindowGeometry(const App::WindowGeometry& geometry);
        /**
         * @brief Gets whether or not to automatically check for application updates.
         * @return True to automatically check for updates, else false
         */
        bool getAutomaticallyCheckForUpdates() const;
        /**
         * @brief Sets whether or not to automatically check for application updates.
         * @param check Whether or not to automatically check for updates
         */
        void setAutomaticallyCheckForUpdates(bool check);
    };
}

#endif //CONFIGURATION_H
