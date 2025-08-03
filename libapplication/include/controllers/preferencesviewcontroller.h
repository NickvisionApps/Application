#ifndef PREFERENCESVIEWCONTROLLER_H
#define PREFERENCESVIEWCONTROLLER_H

#include <string>
#include "models/configuration.h"

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
         * @param configuration The reference to the configuration to use
         */
        PreferencesViewController(Models::Configuration& configuration);
        /**
         * @brief Gets the preferred theme for the application.
         * @return The preferred theme
         */
        Models::Theme getTheme() const;
        /**
         * @brief Sets the preferred theme for the application.
         * @param theme The new preferred theme
         */
        void setTheme(Models::Theme theme);
        /**
         * @brief Gets the available translation languages for the application.
         * @return The list of available translation languages
         */
        const std::vector<std::string>& getAvailableTranslationLanguages() const;
        /**
         * @brief Gets the preferred translation language for the application.
         * @return The preferred translation language
         */
        std::string getTranslationLanguage() const;
        /**
         * @brief Sets the preferred translation language for the application.
         * @param language The new preferred translation language
         */
        void setTranslationLanguage(const std::string& language);
        /**
         * @brief Sets the preferred translation language for the application.
         * @param index The index of the preferred translation language in the available languages list
         */
        void setTranslationLanguage(size_t index);
        /**
         * @brief Saves the current configuration to disk.
         */
        void saveConfiguration();

    private:
        Models::Configuration& m_configuration;
        std::vector<std::string> m_availableTranslationLanguages;
    };
}

#endif //PREFERENCESVIEWCONTROLLER_H