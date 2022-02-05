#pragma once

#include <string>
#include <optional>
#include <wx/wx.h>
#include "Version.h"
#include "UpdateConfig.h"

namespace NickvisionApplication::Models::Update
{
    class Updater
    {
    public:
        Updater(const std::string& linkToConfig, const Version& currentVersion);
        bool UpdateAvailable() const;
        std::optional<Version> GetLatestVersion() const;
        std::string GetChangelog() const;
        bool CheckForUpdates();
        bool Update(wxFrame* mainWindow);

    private:
        std::string m_linkToConfig;
        Version m_currentVersion;
        std::optional<UpdateConfig> m_updateConfig;
        bool m_updateAvailable;
    };
}

