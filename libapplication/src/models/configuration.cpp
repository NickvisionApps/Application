#include "models/configuration.h"
#include <libnick/system/environment.h>

using namespace Nickvision::App;
using namespace Nickvision::System;

namespace Nickvision::Application::Shared::Models
{
    Configuration::Configuration(const std::string& key, const std::string& appName, bool isPortable)
        : DataFileBase{ key, appName, isPortable }
    {
        
    }

    Theme Configuration::getTheme() const
    {
        return m_json["Theme"].is_int64() ? static_cast<Theme>(m_json["Theme"].as_int64()) : Theme::System;
    }

    void Configuration::setTheme(Theme theme)
    {
        m_json["Theme"] = static_cast<int>(theme);
    }

    WindowGeometry Configuration::getWindowGeometry() const
    {
        if(!m_json["WindowGeometry"].is_object())
        {
            return { 800, 600, false };
        }
        return WindowGeometry(m_json["WindowGeometry"].as_object());
    }

    void Configuration::setWindowGeometry(const WindowGeometry& geometry)
    {
        m_json["WindowGeometry"] = geometry.toJson();
    }

    bool Configuration::getAutomaticallyCheckForUpdates() const
    {
        return m_json["AutomaticallyCheckForUpdates"].is_bool() ? m_json["AutomaticallyCheckForUpdates"].as_bool() : Environment::getOperatingSystem() == OperatingSystem::Windows;
    }

    void Configuration::setAutomaticallyCheckForUpdates(bool check)
    {
        m_json["AutomaticallyCheckForUpdates"] = check;
    }
}
