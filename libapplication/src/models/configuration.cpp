#include "models/configuration.h"

using namespace Nickvision::App;

namespace Nickvision::Application::Shared::Models
{
    Configuration::Configuration(const std::string& key, const std::string& appName)
        : DataFileBase{ key, appName }
    {
        
    }

    Theme Configuration::getTheme() const
    {
        const boost::json::value& theme{ m_json["Theme"] };
        if(!theme.is_int64())
        {
            return Theme::System;
        }
        return static_cast<Theme>(theme.as_int64());
    }

    void Configuration::setTheme(Theme theme)
    {
        m_json["Theme"] = static_cast<int>(theme);
    }

    WindowGeometry Configuration::getWindowGeometry() const
    {
        WindowGeometry geometry;
        if(!m_json["WindowGeometry"].is_object())
        {
            geometry.setWidth(800);
            geometry.setHeight(600);
            geometry.setIsMaximized(false);
            return geometry;
        }
        boost::json::object& obj{ m_json["WindowGeometry"].as_object() };
        geometry.setWidth(obj["Width"].is_int64() ? static_cast<long>(obj["Width"].as_int64()) : 800);
        geometry.setHeight(obj["Height"].is_int64() ? static_cast<long>(obj["Height"].as_int64()) : 600);
        geometry.setIsMaximized(obj["IsMaximized"].is_bool() ? obj["IsMaximized"].as_bool() : false);
        return geometry;
    }

    void Configuration::setWindowGeometry(const WindowGeometry& geometry)
    {
        boost::json::object obj;
        obj["Width"] = geometry.getWidth();
        obj["Height"] = geometry.getHeight();
        obj["IsMaximized"] = geometry.isMaximized();
        m_json["WindowGeometry"] = obj;
    }

    bool Configuration::getAutomaticallyCheckForUpdates() const
    {
        const boost::json::value& value{ m_json["AutomaticallyCheckForUpdates"] };
        if(!value.is_bool())
        {
#ifdef _WIN32
            return true;
#else
            return false;
#endif
        }
        return value.as_bool();
    }

    void Configuration::setAutomaticallyCheckForUpdates(bool check)
    {
        m_json["AutomaticallyCheckForUpdates"] = check;
    }
}