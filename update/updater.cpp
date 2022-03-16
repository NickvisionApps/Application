#include "updater.h"
#include <filesystem>
#include <fstream>
#include <unistd.h>
#include <pwd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>

using namespace NickvisionApplication::Update;

Updater::Updater(const std::string& linkToConfig, const Version& currentVersion) : m_linkToConfig(linkToConfig), m_currentVersion(currentVersion), m_updateConfig(std::nullopt), m_updateAvailable(false), m_updateSuccessful(false)
{

}

bool Updater::getUpdateAvailable() const
{
    return m_updateAvailable && m_updateConfig.has_value();
}

std::optional<Version> Updater::getLatestVersion() const
{
    if (m_updateConfig.has_value())
    {
        return m_updateConfig->getLatestVersion();
    }
    return std::nullopt;
}

std::string Updater::getChangelog() const
{
    return m_updateConfig.has_value() ? m_updateConfig->getChangelog() : "";
}

bool Updater::getUpdateSuccessful() const
{
    return m_updateSuccessful;
}

bool Updater::checkForUpdates()
{
    m_updateConfig = UpdateConfig::loadFromUrl(m_linkToConfig);
    if (m_updateConfig.has_value() && m_updateConfig->getLatestVersion() > m_currentVersion)
    {
        m_updateAvailable = true;
    }
    else
    {
        m_updateAvailable = false;
    }
    return getUpdateAvailable();
}

void Updater::update()
{
    if(!getUpdateAvailable())
    {
        m_updateSuccessful = false;
        return;
    }
    std::string downloadsDir = std::string(getpwuid(getuid())->pw_dir) + "/Downloads";
    if(std::filesystem::exists(downloadsDir))
    {
        std::filesystem::create_directories(downloadsDir);
    }
    std::string exePath = downloadsDir + "/NickvisionApplication";
    std::ofstream exeFile(exePath, std::ios::out | std::ios::trunc | std::ios::binary);
    if(exeFile.is_open())
    {
        cURLpp::Easy handle;
        try
        {
            handle.setOpt(cURLpp::Options::Url(m_updateConfig->getLinkToExe()));
            handle.setOpt(cURLpp::Options::FollowLocation(true));
            handle.setOpt(cURLpp::Options::WriteStream(&exeFile));
            handle.perform();
        }
        catch(...)
        {
            m_updateSuccessful = false;
            return;
        }
        exeFile.close();
    }
    else
    {
        m_updateSuccessful = false;
        return;
    }
    chmod(exePath.c_str(), S_IRWXU);
    m_updateSuccessful = true;
}
