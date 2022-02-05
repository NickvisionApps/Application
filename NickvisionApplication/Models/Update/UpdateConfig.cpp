#include "UpdateConfig.h"
#include <fstream>
#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <json/json.h>

namespace NickvisionApplication::Models::Update
{
    UpdateConfig::UpdateConfig() : m_latestVersion("0.0.0"), m_changelog(""), m_linkToExe("")
    {

    }

    std::optional<UpdateConfig> UpdateConfig::LoadFromUrl(const std::string& url)
    {
        cURLpp::Cleanup curlCleanup;
        std::string configFilePath = std::string(getenv("APPDATA")) + "\\Nickvision\\NickvisionApplication\\UpdateConfig.json";
        std::ofstream updateConfigFileOut(configFilePath);
        if (updateConfigFileOut.is_open())
        {
            cURLpp::Easy handle;
            try
            {
                handle.setOpt(cURLpp::Options::Url(url));
                handle.setOpt(cURLpp::Options::FollowLocation(true));
                handle.setOpt(cURLpp::Options::WriteStream(&updateConfigFileOut));
                handle.perform();
            }
            catch (...)
            {
                return std::nullopt;
            }
            updateConfigFileOut.close();
        }
        else
        {
            return std::nullopt;
        }
        std::ifstream updateConfigFileIn(configFilePath);
        if (updateConfigFileIn.is_open())
        {
            UpdateConfig updateConfig;
            Json::Value json;
            try
            {
                updateConfigFileIn >> json;
                updateConfig.SetLatestVersion({ json.get("LatestVersion", "0.0.0").asString() });
                updateConfig.SetChangelog(json.get("Changelog", "").asString());
                updateConfig.SetLinkToExe(json.get("LinkToExe", "").asString());
            }
            catch (...)
            {
                return std::nullopt;
            }
            return updateConfig;
        }
        else
        {
            return std::nullopt;
        }
    }

    const Version& UpdateConfig::GetLatestVersion() const
    {
        return m_latestVersion;
    }

    void UpdateConfig::SetLatestVersion(const Version& latestVersion)
    {
        m_latestVersion = latestVersion;
    }

    const std::string& UpdateConfig::GetChangelog() const
    {
        return m_changelog;
    }

    void UpdateConfig::SetChangelog(const std::string& changelog)
    {
        m_changelog = changelog;
    }

    const std::string& UpdateConfig::GetLinkToExe() const
    {
        return m_linkToExe;
    }

    void UpdateConfig::SetLinkToExe(const std::string& linkToExe)
    {
        m_linkToExe = linkToExe;
    }
}