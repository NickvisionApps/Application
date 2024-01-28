#include "models/configuration.h"
#include <libnick/aura/aura.h>

namespace Nickvision::Application::Shared::Models
{
    Configuration::Configuration(const std::string& key)
        : ConfigurationBase{ key }
    {
    }

    Theme Configuration::getTheme() const
    {
        return static_cast<Theme>(m_json.get("Theme", 0).asInt());
    }

    void Configuration::setTheme(Theme theme)
    {
        m_json["Theme"] = static_cast<int>(theme);
    }

    bool Configuration::getAutomaticallyCheckForUpdates() const
    {
#ifdef _WIN32
        bool def{ true };
#elif defined(__linux__)
        bool def{ false };
#endif
        return m_json.get("AutomaticallyCheckForUpdates", def).asBool();
    }

    void Configuration::setAutomaticallyCheckForUpdates(bool check)
    {
        m_json["AutomaticallyCheckForUpdates"] = check;
    }

    Configuration& Configuration::current()
    {
        return Aura::Aura::getActive().getConfig<Configuration>("config");
    }
}