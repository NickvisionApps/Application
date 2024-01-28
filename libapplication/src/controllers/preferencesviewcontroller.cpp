#include "controllers/preferencesviewcontroller.h"
#include <libnick/aura/aura.h>
#include "models/configuration.h"

using namespace Nickvision::Application::Shared::Models;
using namespace Nickvision::Aura;

namespace Nickvision::Application::Shared::Controllers
{
    const std::string& PreferencesViewController::getId() const
    {
        return Aura::Aura::getActive().getAppInfo().getId();
    }

    Theme PreferencesViewController::getTheme() const
    {
        return Configuration::current().getTheme();
    }

    void PreferencesViewController::setTheme(Theme theme)
    {
        Configuration::current().setTheme(theme);
    }

    bool PreferencesViewController::getAutomaticallyCheckForUpdates() const
    {
        return Configuration::current().getAutomaticallyCheckForUpdates();
    }

    void PreferencesViewController::setAutomaticallyCheckForUpdates(bool check)
    {
        Configuration::current().setAutomaticallyCheckForUpdates(check);
    }

    void PreferencesViewController::saveConfiguration()
    {
        Configuration::current().save();
    }
}