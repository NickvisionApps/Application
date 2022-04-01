#ifndef UPDATER_H
#define UPDATER_H

#include <mutex>
#include <string>
#include <optional>
#include "version.h"
#include "updateconfig.h"

namespace NickvisionApplication::Update
{
    class Updater
    {
    public:
        Updater(const std::string& linkToConfig, const Version& currentVersion);
        bool getUpdateAvailable() const;
        std::optional<Version> getLatestVersion() const;
        std::string getChangelog() const;
        bool getUpdateSuccessful() const;
        bool checkForUpdates();
        void update();

    private:
        bool validateUpdate(const std::string& pathToUpdate);
        mutable std::mutex m_mutex;
        std::string m_linkToConfig;
        Version m_currentVersion;
        std::optional<UpdateConfig> m_updateConfig;
        bool m_updateAvailable;
        bool m_updateSuccessful;
    };
}

#endif // UPDATER_H
