#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <filesystem>
#include <string>
#include <libnick/app/windowgeometry.h>
#include <libnick/helpers/jsonfilebase.h>
#include <libnick/update/versiontype.h>
#include "theme.h"

namespace Nickvision::Application::Shared::Models
{
    /**
     * @brief A model for the configuration of the application.
     */
    class Configuration : public Helpers::JsonFileBase
    {
    public:
        /**
         * @brief Constructs a Configuration.
         * @param path The path to the configuration file
         */
        Configuration(const std::filesystem::path& path);
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
         * @brief Gets the preferred update type for the application.
         * @return The preferred update type
         */
        Update::VersionType getPreferredUpdateType() const;
        /**
         * @brief Sets the preferred update type for the application.
         * @param type The new preferred update type
         */
        void setPreferredUpdateType(Update::VersionType type);
    };
}

#endif //CONFIGURATION_H
