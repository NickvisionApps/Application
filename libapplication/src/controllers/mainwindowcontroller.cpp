#include "controllers/mainwindowcontroller.h"
#include <ctime>
#include <sstream>
#include <thread>
#include <libnick/filesystem/userdirectories.h>
#include <libnick/helpers/stringhelpers.h>
#include <libnick/localization/gettext.h>
#include <libnick/notifications/appnotification.h>
#include <libnick/system/environment.h>
#include "models/configuration.h"

#define CONFIG_FILE_KEY "config"

using namespace Nickvision::App;
using namespace Nickvision::Application::Shared::Models;
using namespace Nickvision::Events;
using namespace Nickvision::Filesystem;
using namespace Nickvision::Localization;
using namespace Nickvision::Notifications;
using namespace Nickvision::System;
using namespace Nickvision::Update;

namespace Nickvision::Application::Shared::Controllers
{
    MainWindowController::MainWindowController(const std::vector<std::string>& args)
        : m_started{ false },
        m_args{ args },
        m_appInfo{ "org.nickvision.application", "Nickvision Application", "Application" },
#ifdef PORTABLE_BUILD
        m_dataFileManager{ m_appInfo.getName(), true }
#else
        m_dataFileManager{ m_appInfo.getName(), false }
#endif
    {
        m_appInfo.setVersion({ "2025.8.0-next" });
        m_appInfo.setShortName(_("Application"));
        m_appInfo.setDescription(_("Create new Nickvision applications"));
        m_appInfo.setChangelog("- Initial Release");
        m_appInfo.setSourceRepo("https://github.com/NickvisionApps/Application");
        m_appInfo.setIssueTracker("https://github.com/NickvisionApps/Application/issues/new");
        m_appInfo.setSupportUrl("https://github.com/NickvisionApps/Application/discussions");
        m_appInfo.getExtraLinks()[_("Matrix Chat")] = "https://matrix.to/#/#nickvision:matrix.org";
        m_appInfo.getDevelopers()["Nicholas Logozzo"] = "https://github.com/nlogozzo";
        m_appInfo.getDevelopers()[_("Contributors on GitHub")] = "https://github.com/NickvisionApps/Application/graphs/contributors";
        m_appInfo.getDesigners()["Nicholas Logozzo"] = "https://github.com/nlogozzo";
        m_appInfo.getDesigners()[_("Fyodor Sobolev")] = "https://github.com/fsobolev";
        m_appInfo.getDesigners()["DaPigGuy"] = "https://github.com/DaPigGuy";
        m_appInfo.getArtists()[_("David Lapshin")] = "https://github.com/daudix";
        m_appInfo.setTranslatorCredits(_("translator-credits"));
        Gettext::init(m_appInfo.getEnglishShortName());
        std::string translationLanguage{ m_dataFileManager.get<Configuration>(CONFIG_FILE_KEY).getTranslationLanguage() };
        if(!translationLanguage.empty())
        {
            Gettext::changeLanguage(translationLanguage);
        }
        m_updater = std::make_shared<Updater>(m_appInfo.getSourceRepo());
    }

    Event<EventArgs>& MainWindowController::configurationSaved()
    {
        return m_dataFileManager.get<Configuration>(CONFIG_FILE_KEY).saved();
    }

    Event<NotificationSentEventArgs>& MainWindowController::notificationSent()
    {
        return AppNotification::sent();
    }

    Event<ParamEventArgs<Version>>& MainWindowController::appUpdateAvailable()
    {
        return m_appUpdateAvailable;
    }

    Event<ParamEventArgs<double>>& MainWindowController::appUpdateProgressChanged()
    {
        return m_appUpdateProgressChanged;
    }

    const AppInfo& MainWindowController::getAppInfo() const
    {
        return m_appInfo;
    }

    Theme MainWindowController::getTheme()
    {
        return m_dataFileManager.get<Configuration>(CONFIG_FILE_KEY).getTheme();
    }

    VersionType MainWindowController::getPreferredUpdateType()
    {
        return m_dataFileManager.get<Configuration>(CONFIG_FILE_KEY).getPreferredUpdateType();
    }

    void MainWindowController::setPreferredUpdateType(VersionType type)
    {
        m_dataFileManager.get<Configuration>(CONFIG_FILE_KEY).setPreferredUpdateType(type);
    }

    std::string MainWindowController::getDebugInformation(const std::string& extraInformation) const
    {
        std::stringstream builder;
        //Extra
        if(!extraInformation.empty())
        {
            builder << extraInformation << std::endl;
#ifdef __linux__
            builder << Environment::exec("locale");
#endif
        }
        return Environment::getDebugInformation(m_appInfo, builder.str());
    }

    bool MainWindowController::canShutdown() const
    {
        return true;
    }

    std::shared_ptr<PreferencesViewController> MainWindowController::createPreferencesViewController()
    {
        return std::make_shared<PreferencesViewController>(m_dataFileManager.get<Configuration>(CONFIG_FILE_KEY));
    }

#ifdef _WIN32
    const StartupInformation& MainWindowController::startup(HWND hwnd)
#elif defined(__linux__)
    const StartupInformation& MainWindowController::startup(const std::string& desktopFile)
#else
    const StartupInformation& MainWindowController::startup()
#endif
    {
        static StartupInformation info;
        if (m_started)
        {
            return info;
        }
        //Load configuration
        info.setWindowGeometry(m_dataFileManager.get<Configuration>(CONFIG_FILE_KEY).getWindowGeometry());
        //Load taskbar item
#ifdef _WIN32
        m_taskbar.connect(hwnd);
#elif defined(__linux__)
        m_taskbar.connect(desktopFile);
#endif
        //Start checking for app updates
        std::thread workerUpdates{ [this]()
        {
            Version latest{ m_updater->fetchCurrentVersion(getPreferredUpdateType()) };
            if(!latest.empty())
            {
                if(latest > m_appInfo.getVersion())
                {
                    m_appUpdateAvailable.invoke({ latest });
                }
            }
        } };
        workerUpdates.detach();
        m_started = true;
        return info;
    }

    void MainWindowController::shutdown(const WindowGeometry& geometry)
    {
        Configuration& config{ m_dataFileManager.get<Configuration>(CONFIG_FILE_KEY) };
        config.setWindowGeometry(geometry);
        config.save();
    }

#ifdef _WIN32
    void MainWindowController::startWindowsUpdate()
    {
        std::thread worker{ [this]()
        {
            m_appUpdateProgressChanged.invoke({ 0.0 });
            bool res{ m_updater->windowsUpdate(getPreferredUpdateType(), { [this](curl_off_t downloadTotal, curl_off_t downloadNow, curl_off_t, curl_off_t, intptr_t) -> bool
            {
                if(downloadTotal == 0)
                {
                    return true;
                }
                m_appUpdateProgressChanged.invoke({ static_cast<double>(static_cast<long double>(downloadNow) / static_cast<long double>(downloadTotal)) });
                return true;
            } }) };
            if(res)
            {
                m_appUpdateProgressChanged.invoke({ 1.0 });
            }
            else
            {
                AppNotification::send({ _("Unable to download and install update"), NotificationSeverity::Error });
            }
        } };
        worker.detach();
    }
#endif

    std::string MainWindowController::getGreeting() const
    {
        std::time_t now{ std::time(nullptr) };
        std::tm* cal{ std::localtime(&now) };
        if (cal->tm_hour >= 0 && cal->tm_hour < 6)
        {
            return _p("Night", "Good Morning!");
        }
        else if (cal->tm_hour < 12)
        {
            return _p("Morning", "Good Morning!");
        }
        else if (cal->tm_hour < 18)
        {
            return _("Good Afternoon!");
        }
        else if (cal->tm_hour < 24)
        {
            return _("Good Evening!");
        }
        return _("Good Day!");
    }

    const std::filesystem::path& MainWindowController::getFolderPath() const
    {
        return m_folderPath;
    }

    const std::vector<std::filesystem::path>& MainWindowController::getFiles() const
    {
        return m_files;
    }

    bool MainWindowController::isFolderOpened() const
    {
        return std::filesystem::exists(m_folderPath) && std::filesystem::is_directory(m_folderPath);
    }

    Event<EventArgs>& MainWindowController::folderChanged()
    {
        return m_folderChanged;
    }

    bool MainWindowController::openFolder(const std::filesystem::path& path)
    {
        m_folderPath = path;
        if (!isFolderOpened())
        {
            return false;
        }
        //Load Files
        m_files.clear();
        if (std::filesystem::exists(m_folderPath))
        {
            for (const std::filesystem::directory_entry& e : std::filesystem::directory_iterator(m_folderPath))
            {
                if (e.is_regular_file())
                {
                    m_files.push_back(e.path());
                }
            }
        }
        //UI
        AppNotification::send({ _f("Folder Opened: {}", m_folderPath.string()), NotificationSeverity::Success, "close" });
        m_folderChanged.invoke({});
        m_taskbar.setCount(static_cast<long>(m_files.size()));
        m_taskbar.setCountVisible(true);
        return true;
    }

    void MainWindowController::closeFolder()
    {
        if(!isFolderOpened())
        {
            return;
        }
        m_folderPath = std::filesystem::path();
        m_files.clear();
        AppNotification::send({ _("Folder closed"), NotificationSeverity::Warning });
        m_folderChanged.invoke({});
        m_taskbar.setCountVisible(false);
    }
}
