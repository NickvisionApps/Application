#include "controllers/preferencesviewcontroller.h"
#include <libnick/aura/aura.h>
#include "models/configuration.h"

using namespace Nickvision::Application::Shared::Models;
using namespace Nickvision::Aura;

namespace Nickvision::Application::Shared::Controllers
{
    const std::string& PreferencesViewController::getId() const noexcept
    {
        return Aura::Aura::getActive().getAppInfo().getId();
    }

	Theme PreferencesViewController::getTheme() const noexcept
	{
		return Configuration::current().getTheme();
	}

	void PreferencesViewController::setTheme(Theme theme) noexcept
	{
		Configuration::current().setTheme(theme);
	}

	bool PreferencesViewController::getAutomaticallyCheckForUpdates() const noexcept
	{
		return Configuration::current().getAutomaticallyCheckForUpdates();
	}

	void PreferencesViewController::setAutomaticallyCheckForUpdates(bool check) noexcept
	{
		Configuration::current().setAutomaticallyCheckForUpdates(check);
	}

	void PreferencesViewController::saveConfiguration() noexcept
	{
		Configuration::current().save();
	}
}