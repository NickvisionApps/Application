#include "controllers/preferencesviewcontroller.h"
#include <algorithm>
#include <libnick/localization/gettext.h>

using namespace Nickvision::Application::Shared::Models;
using namespace Nickvision::Localization;

namespace Nickvision::Application::Shared::Controllers
{
    PreferencesViewController::PreferencesViewController(Configuration& configuration)
        : m_configuration{ configuration },
        m_availableTranslationLanguages{ Gettext::getAvailableLanguages() }
    {
        m_availableTranslationLanguages.push_back("en_US");
        std::sort(m_availableTranslationLanguages.begin(), m_availableTranslationLanguages.end());
        m_availableTranslationLanguages.insert(m_availableTranslationLanguages.begin(), _("System"));
    }

    Theme PreferencesViewController::getTheme() const
    {
        return m_configuration.getTheme();
    }

    void PreferencesViewController::setTheme(Theme theme)
    {
        m_configuration.setTheme(theme);
    }

    const std::vector<std::string>& PreferencesViewController::getAvailableTranslationLanguages() const
    {
        return m_availableTranslationLanguages;
    }

    std::string PreferencesViewController::getTranslationLanguage() const
    {
        std::string language{ m_configuration.getTranslationLanguage() };
        if(language.empty())
        {
            return _("System");
        }
        else if(language == "C")
        {
            return "en_US";
        }
        return language;
    }

    void PreferencesViewController::setTranslationLanguage(const std::string& language)
    {
        if(language == _("System"))
        {
            m_configuration.setTranslationLanguage("");
        }
        else if(language == "en_US")
        {
            m_configuration.setTranslationLanguage("C");
        }
        else
        {
            m_configuration.setTranslationLanguage(language);
        }
    }

    void PreferencesViewController::setTranslationLanguage(size_t index)
    {
        if(index == 1 || index >= m_availableTranslationLanguages.size())
        {
            m_configuration.setTranslationLanguage("");
        }
        else if(index == 0)
        {
            m_configuration.setTranslationLanguage("C");
        }
        else
        {
            m_configuration.setTranslationLanguage(m_availableTranslationLanguages[index]);
        }
    }

    bool PreferencesViewController::getAutomaticallyCheckForUpdates() const
    {
        return m_configuration.getAutomaticallyCheckForUpdates();
    }

    void PreferencesViewController::setAutomaticallyCheckForUpdates(bool check)
    {
        m_configuration.setAutomaticallyCheckForUpdates(check);
    }

    void PreferencesViewController::saveConfiguration()
    {
        m_configuration.save();
    }
}