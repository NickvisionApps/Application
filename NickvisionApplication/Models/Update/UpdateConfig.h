#pragma once

#include <string>
#include <optional>
#include "Version.h"

namespace NickvisionApplication::Models::Update
{
    class UpdateConfig
    {
    public:
        static std::optional<UpdateConfig> LoadFromUrl(const std::string& url);
        const Version& GetLatestVersion() const;
        void SetLatestVersion(const Version& latestVersion);
        const std::string& GetChangelog() const;
        void SetChangelog(const std::string& changelog);
        const std::string& GetLinkToExe() const;
        void SetLinkToExe(const std::string& linkToExe);

    private:
        UpdateConfig();
        Version m_latestVersion;
        std::string m_changelog;
        std::string m_linkToExe;
    };
}

