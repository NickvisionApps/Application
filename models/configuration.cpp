#include "configuration.h"
#include <filesystem>
#include <fstream>
#include <unistd.h>
#include <pwd.h>
#include <json/json.h>

using namespace NickvisionApplication::Models;

Configuration::Configuration() : m_configDir(std::string(getpwuid(getuid())->pw_dir) + "/.config/Nickvision/NickvisionApplication/"), m_isFirstTimeOpen(true), m_theme(Theme::System)
{
    if (!std::filesystem::exists(m_configDir))
    {
        std::filesystem::create_directories(m_configDir);
    }
    std::ifstream configFile(m_configDir + "config.json");
    if (configFile.is_open())
    {
        Json::Value json;
        try
        {
            configFile >> json;
            m_isFirstTimeOpen = json.get("IsFirstTimeOpen", true).asBool();
            m_theme = static_cast<Theme>(json.get("Theme", 0).asInt());
        }
        catch (...) { }
    }
}

Configuration& Configuration::getInstance()
{
    static Configuration instance;
    return instance;
}

bool Configuration::getIsFirstTimeOpen() const
{
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_isFirstTimeOpen;
}

void Configuration::setIsFirstTimeOpen(bool isFirstTimeOpen)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_isFirstTimeOpen = isFirstTimeOpen;
}

Theme Configuration::getTheme() const
{
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_theme;
}

void Configuration::setTheme(Theme theme)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_theme = theme;
}

void Configuration::save() const
{
    std::lock_guard<std::mutex> lock(m_mutex);
    std::ofstream configFile(m_configDir + "config.json");
    if (configFile.is_open())
    {
        Json::Value json;
        json["IsFirstTimeOpen"] = m_isFirstTimeOpen;
        json["Theme"] = static_cast<int>(m_theme);
        configFile << json;
    }
}
