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
            setIsFirstTimeOpen(json.get("IsFirstTimeOpen", true).asBool());
            setTheme(static_cast<Theme>(json.get("Theme", 0).asInt()));
        }
        catch (...) { }
    }
}

bool Configuration::getIsFirstTimeOpen() const
{
    return m_isFirstTimeOpen;
}

void Configuration::setIsFirstTimeOpen(bool isFirstTimeOpen)
{
    m_isFirstTimeOpen = isFirstTimeOpen;
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
    std::ofstream configFile(m_configDir + "config.json");
    if (configFile.is_open())
    {
        Json::Value json;
        json["IsFirstTimeOpen"] = getIsFirstTimeOpen();
        json["Theme"] = static_cast<int>(getTheme());
        configFile << json;
    }
}
