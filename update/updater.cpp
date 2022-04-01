#include "updater.h"
#include <filesystem>
#include <fstream>
#include <unistd.h>
#include <pwd.h>
#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>

using namespace NickvisionApplication::Update;

Updater::Updater(const std::string& linkToConfig, const Version& currentVersion) : m_linkToConfig(linkToConfig), m_currentVersion(currentVersion), m_updateConfig(std::nullopt), m_updateAvailable(false), m_updateSuccessful(false)
{

}

bool Updater::getUpdateAvailable() const
{
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_updateAvailable;
}

std::optional<Version> Updater::getLatestVersion() const
{
    std::lock_guard<std::mutex> lock(m_mutex);
    if (m_updateConfig.has_value())
    {
        return m_updateConfig->getLatestVersion();
    }
    return std::nullopt;
}

std::string Updater::getChangelog() const
{
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_updateConfig.has_value() ? m_updateConfig->getChangelog() : "";
}

bool Updater::getUpdateSuccessful() const
{
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_updateSuccessful;
}

bool Updater::checkForUpdates()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_updateConfig = UpdateConfig::loadFromUrl(m_linkToConfig);
    if (m_updateConfig.has_value() && m_updateConfig->getLatestVersion() > m_currentVersion)
    {
        m_updateAvailable = true;
    }
    else
    {
        m_updateAvailable = false;
    }
    return m_updateAvailable;
}

void Updater::update()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    if(!m_updateAvailable)
    {
        m_updateSuccessful = false;
        return;
    }
    std::string downloadsDir = std::string(getpwuid(getuid())->pw_dir) + "/Downloads";
    if(std::filesystem::exists(downloadsDir))
    {
        std::filesystem::create_directories(downloadsDir);
    }
    std::string tarGzPath = downloadsDir + "/NickvisionApplication.tar.gz";
    std::ofstream exeFile(tarGzPath, std::ios::out | std::ios::trunc | std::ios::binary);
    if(exeFile.is_open())
    {
        cURLpp::Cleanup cleanup;
        cURLpp::Easy handle;
        try
        {
            handle.setOpt(cURLpp::Options::Url(m_updateConfig->getLinkToTarGz()));
            handle.setOpt(cURLpp::Options::FollowLocation(true));
            handle.setOpt(cURLpp::Options::WriteStream(&exeFile));
            handle.perform();
        }
        catch(...)
        {
            m_updateSuccessful = false;
            return;
        }
    }
    else
    {
        m_updateSuccessful = false;
        return;
    }
    m_updateSuccessful = true;
}
