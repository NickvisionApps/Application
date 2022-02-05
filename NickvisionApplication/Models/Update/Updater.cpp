#include "Updater.h"
#include <filesystem>
#include <fstream>
#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>

namespace NickvisionApplication::Models::Update
{
    Updater::Updater(const std::string& linkToConfig, const Version& currentVersion) : m_linkToConfig(linkToConfig), m_currentVersion(currentVersion), m_updateConfig(std::nullopt), m_updateAvailable(false)
    {

    }

    bool Updater::UpdateAvailable() const
    {
        return m_updateAvailable && m_updateConfig.has_value();
    }

    std::optional<Version> Updater::GetLatestVersion() const
    {
        if (m_updateConfig.has_value())
        {
            return m_updateConfig->GetLatestVersion();
        }
        return std::nullopt;
    }

    std::string Updater::GetChangelog() const
    {
        return m_updateConfig.has_value() ? m_updateConfig->GetChangelog() : "";
    }

    bool Updater::CheckForUpdates()
    {
        m_updateConfig = UpdateConfig::LoadFromUrl(m_linkToConfig);
        if (m_updateConfig.has_value() && m_updateConfig->GetLatestVersion() > m_currentVersion)
        {
            m_updateAvailable = true;
        }
        return UpdateAvailable();
    }

    bool Updater::Update(wxFrame* mainWindow)
    {
        if (!UpdateAvailable())
        {
            return false;
        }
        std::string exePath = std::string(getenv("APPDATA")) + "\\Nickvision\\NickvisionApplication\\Setup.exe";
        std::ofstream exeFile(exePath, std::ios::out | std::ios::trunc | std::ios::binary);
        if (exeFile.is_open())
        {
            cURLpp::Easy handle;
            try
            {
                handle.setOpt(cURLpp::Options::Url(m_updateConfig->GetLinkToExe()));
                handle.setOpt(cURLpp::Options::FollowLocation(true));
                handle.setOpt(cURLpp::Options::WriteStream(&exeFile));
                handle.perform();
            }
            catch (...)
            {
                return false;
            }
            exeFile.close();
        }
        else
        {
            return false;
        }
        wxExecute(exePath, wxEXEC_ASYNC | wxEXEC_HIDE_CONSOLE);
        mainWindow->Close();
        return true;
    }
}