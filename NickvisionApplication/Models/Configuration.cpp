#include "Configuration.h"
#include <filesystem>
#include <fstream>
#include <QStandardPaths>
#include <json/json.h>

namespace NickvisionApplication::Models
{
    Configuration::Configuration() : m_configDir{ QStandardPaths::writableLocation(QStandardPaths::AppDataLocation).toStdString() + "/Nickvision/NickvisionApplication/" }, m_theme{ Theme::Light }
    {
        if (!std::filesystem::exists(m_configDir))
        {
            std::filesystem::create_directories(m_configDir);
        }
        std::ifstream configFile{ m_configDir + "config.json" };
        if (configFile.is_open())
        {
            Json::Value json;
            configFile >> json;
            m_theme = static_cast<Theme>(json.get("Theme", 0).asInt());
        }
    }

    Configuration& Configuration::getInstance()
    {
        static Configuration instance;
        return instance;
    }

    Theme Configuration::getTheme() const
    {
        return m_theme;
    }

    void Configuration::setTheme(Theme theme)
    {
        m_theme = theme;
    }

    void Configuration::save() const
    {
        std::ofstream configFile{ m_configDir + "config.json" };
        if (configFile.is_open())
        {
            Json::Value json;
            json["Theme"] = static_cast<int>(m_theme);
            configFile << json;
        }
    }
}