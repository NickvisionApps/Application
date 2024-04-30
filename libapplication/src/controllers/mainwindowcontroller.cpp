#include "controllers/mainwindowcontroller.h"
#include <ctime>
#include <format>
#include <locale>
#include <sstream>
#include <thread>
#include <libnick/app/aura.h>
#include <libnick/helpers/codehelpers.h>
#include <libnick/helpers/stringhelpers.h>
#include <libnick/localization/gettext.h>
#include "models/configuration.h"
#ifdef _WIN32
#include <windows.h>
#endif

using namespace Nickvision::Application::Shared::Models;
using namespace Nickvision::App;
using namespace Nickvision::Events;
using namespace Nickvision::Notifications;
using namespace Nickvision::Update;

namespace Nickvision::Application::Shared::Controllers
{
    MainWindowController::MainWindowController()
        : m_started{ false }
    {
        Aura::getActive().init("org.nickvision.application", "Nickvision Application", "Application");
        AppInfo& appInfo{ Aura::getActive().getAppInfo() };
        appInfo.setVersion({ "2024.3.0-next" });
        appInfo.setShortName(_("Application"));
        appInfo.setDescription(_("Create new Nickvision applications"));
        appInfo.setSourceRepo("https://github.com/NickvisionApps/Application");
        appInfo.setIssueTracker("https://github.com/NickvisionApps/Application/issues/new");
        appInfo.setSupportUrl("https://github.com/NickvisionApps/Application/discussions");
        appInfo.getExtraLinks()[_("Matrix Chat")] = "https://matrix.to/#/#nickvision:matrix.org";
        appInfo.getDevelopers()["Nicholas Logozzo"] = "https://github.com/nlogozzo";
        appInfo.getDevelopers()[_("Contributors on GitHub")] = "https://github.com/NickvisionApps/Application/graphs/contributors";
        appInfo.getDesigners()["Nicholas Logozzo"] = "https://github.com/nlogozzo";
        appInfo.getDesigners()[_("Fyodor Sobolev")] = "https://github.com/fsobolev";
        appInfo.getDesigners()["DaPigGuy"] = "https://github.com/DaPigGuy";
        appInfo.getArtists()[_("David Lapshin")] = "https://github.com/daudix";
        appInfo.setTranslatorCredits(_("translator-credits"));
    }

    AppInfo& MainWindowController::getAppInfo() const
    {
        return Aura::getActive().getAppInfo();
    }

    bool MainWindowController::isDevVersion() const
    {
        return Aura::getActive().getAppInfo().getVersion().getVersionType() == VersionType::Preview;
    }

    Theme MainWindowController::getTheme() const
    {
        return Aura::getActive().getConfig<Configuration>("config").getTheme();
    }

    Event<EventArgs>& MainWindowController::configurationSaved()
    {
        return Aura::getActive().getConfig<Configuration>("config").saved();
    }

    Event<NotificationSentEventArgs>& MainWindowController::notificationSent()
    {
        return m_notificationSent;
    }

    Event<ShellNotificationSentEventArgs>& MainWindowController::shellNotificationSent()
    {
        return m_shellNotificationSent;
    }

    std::string MainWindowController::getDebugInformation(const std::string& extraInformation) const
    {
        std::stringstream builder;
        builder << Aura::getActive().getAppInfo().getId();
#ifdef _WIN32
        builder << ".winui" << std::endl;
#elif defined(__linux__)
        builder << ".gnome" << std::endl;
#endif
        builder << Aura::getActive().getAppInfo().getVersion().toString() << std::endl << std::endl;
        if(Aura::getActive().isRunningViaFlatpak())
        {
            builder << "Running under Flatpak" << std::endl;
        }
        else if(Aura::getActive().isRunningViaSnap())
        {
            builder << "Running under Snap" << std::endl;
        }
        else
        {
            builder << "Running locally" << std::endl;
        }
#ifdef _WIN32
        LCID lcid = GetThreadLocale();
        wchar_t name[LOCALE_NAME_MAX_LENGTH];
        if(LCIDToLocaleName(lcid, name, LOCALE_NAME_MAX_LENGTH, 0) > 0)
        {
            builder << StringHelpers::toString(name) << std::endl;
        }
#elif defined(__linux__)
        builder << std::locale("").name() << std::endl;
#endif
        if (!extraInformation.empty())
        {
            builder << extraInformation << std::endl;
        }
        return builder.str();
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

    std::shared_ptr<PreferencesViewController> MainWindowController::createPreferencesViewController() const
    {
        return std::make_shared<PreferencesViewController>();
    }

    void MainWindowController::startup()
    {
        if (!m_started)
        {
#ifdef _WIN32
            try
            {
                m_updater = std::make_shared<Updater>(Aura::getActive().getAppInfo().getSourceRepo());
            }
            catch(...)
            {
                m_updater = nullptr;
            }
            if (Aura::getActive().getConfig<Configuration>("config").getAutomaticallyCheckForUpdates())
            {
                checkForUpdates();
            }
#endif
            m_started = true;
        }
    }

    void MainWindowController::checkForUpdates()
    {
        if(m_updater)
        {
            std::thread worker{ [&]()
            {
                Version latest{ m_updater->fetchCurrentStableVersion() };
                if (!latest.empty())
                {
                    if (latest > Aura::getActive().getAppInfo().getVersion())
                    {
                        m_notificationSent.invoke({ _("New update available"), NotificationSeverity::Success, "update" });
                    }
                }
            } };
            worker.detach();
        }
    }

#ifdef _WIN32
    void MainWindowController::windowsUpdate()
    {
        if(m_updater)
        {
            std::thread worker{ [&]()
            {
                bool res{ m_updater->windowsUpdate(VersionType::Stable) };
                if (!res)
                {
                    m_notificationSent.invoke({ _("Unable to download and install update"), NotificationSeverity::Error, "error" });
                }
            } };
            worker.detach();
        }
    }

    bool MainWindowController::connectTaskbar(HWND hwnd)
    {
        return m_taskbar.connect(hwnd);
    }
#elif defined(__linux__)
    bool MainWindowController::connectTaskbar(const std::string& desktopFile)
    {
        return m_taskbar.connect(desktopFile);
    }
#endif

    bool MainWindowController::openFolder(const std::filesystem::path& path)
    {
        if (std::filesystem::exists(path) && std::filesystem::is_directory(path))
        {
            m_folderPath = path;
            loadFiles();
            m_notificationSent.invoke({ std::vformat(_("Folder Opened: {}"), std::make_format_args(CodeHelpers::unmove(m_folderPath.string()))), NotificationSeverity::Success, "close" });
            m_folderChanged.invoke({});
            m_taskbar.setCount(static_cast<long>(m_files.size()));
            m_taskbar.setCountVisible(true);
            return true;
        }
        return false;
    }

    void MainWindowController::closeFolder()
    {
        m_folderPath = std::filesystem::path();
        m_files.clear();
        m_notificationSent.invoke({ _("Folder closed"), NotificationSeverity::Warning });
        m_folderChanged.invoke({});
        m_taskbar.setCountVisible(false);
    }

    void MainWindowController::loadFiles()
    {
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
    }
}
