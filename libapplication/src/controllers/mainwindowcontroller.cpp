#include "controllers/mainwindowcontroller.h"
#include <ctime>
#include <format>
#include <sstream>
#include <thread>
#include <boost/locale.hpp>
#include <libnick/aura/aura.h>
#include <libnick/helpers/stringhelpers.h>
#include <libnick/localization/gettext.h>
#include "models/configuration.h"

using namespace Nickvision::Application::Shared::Models;
using namespace Nickvision::Aura;
using namespace Nickvision::Events;
using namespace Nickvision::Notifications;
using namespace Nickvision::Update;

namespace Nickvision::Application::Shared::Controllers
{
	MainWindowController::MainWindowController(const std::vector<std::string>& args) noexcept
		: m_updater{ "https://github.com/NickvisionApps/Application" }
	{
		Aura::Aura::init("org.nickvision.application", "Nickvision Application", "Application");
		AppInfo& appInfo{ Aura::Aura::getActive().getAppInfo() };
		appInfo.setVersion({ "2024.1.0-next" });
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
		if (args.size() > 0 && std::filesystem::exists(args[0]) && std::filesystem::is_directory(args[0]))
		{
			m_folderPath = args[0];
		}
	}

	AppInfo& MainWindowController::getAppInfo() const noexcept
	{
		return Aura::Aura::getActive().getAppInfo();
	}

    bool MainWindowController::isDevVersion() const noexcept
    {
        return Aura::Aura::getActive().getAppInfo().getVersion().getVersionType() == VersionType::Preview;
    }

	Theme MainWindowController::getTheme() const noexcept
	{
		return Configuration::current().getTheme();
	}

	Event<NotificationSentEventArgs>& MainWindowController::notificationSent() noexcept
	{
		return m_notificationSent;
	}

	Event<ShellNotificationSentEventArgs>& MainWindowController::shellNotificationSent() noexcept
	{
		return m_shellNotificationSent;
	}

    std::string MainWindowController::getDebugInformation(const std::string& extraInformation) const noexcept
    {
        std::stringstream builder;
        builder << Aura::Aura::getActive().getAppInfo().getId();
#ifdef _WIN32
        builder << ".winui" << std::endl;
#elif defined(__linux__)
        builder << ".gnome" << std::endl;
#endif
        builder << Aura::Aura::getActive().getAppInfo().getVersion().toString() << std::endl << std::endl;
        builder << StringHelpers::split(boost::locale::util::get_system_locale(), ".")[0] << std::endl;
        if(!extraInformation.empty())
        {
            builder << extraInformation << std::endl;
        }
        return builder.str();
    }

	const std::filesystem::path& MainWindowController::getFolderPath() const noexcept
	{
		return m_folderPath;
	}

	const std::vector<std::filesystem::path> MainWindowController::getFiles() const noexcept
	{
		return m_files;
	}

	bool MainWindowController::isFolderOpened() const noexcept
	{
		return std::filesystem::exists(m_folderPath) && std::filesystem::is_directory(m_folderPath);
	}

	Event<EventArgs>& MainWindowController::folderChanged() noexcept
	{
		return m_folderChanged;
	}

	std::string MainWindowController::getGreeting() const noexcept
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

    std::shared_ptr<PreferencesViewController> MainWindowController::createPreferencesViewController() const noexcept
    {
        return std::make_shared<PreferencesViewController>();
    }

	void MainWindowController::startup() noexcept
	{
		static bool started{ false };
		if (!started)
		{
#ifdef _WIN32
			if (Configuration::current().getAutomaticallyCheckForUpdates())
			{
				checkForUpdates();
			}
#endif
			loadFiles();
			started = true;
		}
	}

	void MainWindowController::checkForUpdates() noexcept
	{
		std::thread worker{ [&]()
		{
			Version latest{ m_updater.fetchCurrentStableVersion() };
			if (!latest.empty())
			{
				if (latest > Aura::Aura::getActive().getAppInfo().getVersion())
				{
					m_notificationSent.invoke({ _("New update available"), NotificationSeverity::Success, "update" });
				}
			}
		} };
		worker.detach();
	}

#ifdef _WIN32
	void MainWindowController::windowsUpdate() noexcept
	{
		std::thread worker{ [&]()
		{
			bool res{ m_updater.windowsUpdate(VersionType::Stable) };
			if (!res)
			{
				m_notificationSent.invoke({ _("Unable to download and install update"), NotificationSeverity::Error, "error" });
			}
		} };
        worker.detach();
	}

	bool MainWindowController::connectTaskbar(HWND hwnd) noexcept
	{
		return m_taskbar.connect(hwnd);
	}
#elif defined(__linux__)
	bool MainWindowController::connectTaskbar(const std::string& desktopFile) noexcept
	{
		return m_taskbar.connect(desktopFile);
	}
#endif

	bool MainWindowController::openFolder(const std::filesystem::path& path) noexcept
	{
		if (std::filesystem::exists(path) && std::filesystem::is_directory(path))
		{
			m_folderPath = path;
			loadFiles();
			m_notificationSent.invoke({ std::vformat(_("Folder Opened: {}"), std::make_format_args(m_folderPath.string())), NotificationSeverity::Success, "close" });
            m_folderChanged.invoke({});
			m_taskbar.setCount(static_cast<long>(m_files.size()));
			return true;
		}
		return false;
	}

	void MainWindowController::closeFolder() noexcept
	{
		m_folderPath = std::filesystem::path();
		m_files.clear();
		m_notificationSent.invoke({ _("Folder closed"), NotificationSeverity::Warning });
		m_folderChanged.invoke({});
		m_taskbar.setCount(-1L);
	}

	void MainWindowController::loadFiles() noexcept
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