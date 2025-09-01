#include "models/configuration.h"

using namespace Nickvision::App;
using namespace Nickvision::Helpers;
using namespace Nickvision::Update;

namespace Nickvision::Application::Shared::Models
{
    Configuration::Configuration(const std::filesystem::path& path)
        : JsonFileBase{ path }
    {

    }

    Theme Configuration::getTheme() const
    {
        return static_cast<Theme>(get("Theme", static_cast<int>(Theme::System)));
    }

    void Configuration::setTheme(Theme theme)
    {
        set("Theme", static_cast<int>(theme));
    }

    std::string Configuration::getTranslationLanguage() const
    {
        return get<std::string>("TranslationLanguage", "");
    }

    void Configuration::setTranslationLanguage(const std::string& language)
    {
        set("TranslationLanguage", language);
    }

    WindowGeometry Configuration::getWindowGeometry() const
    {
        boost::json::object geometry = get<boost::json::object>("WindowGeometry", {});
        if(geometry.empty())
        {
            return { 800, 600, false };
        }
        return WindowGeometry(geometry);
    }

    void Configuration::setWindowGeometry(const WindowGeometry& geometry)
    {
        set("WindowGeometry", geometry.toJson().as_object());
    }

    VersionType Configuration::getPreferredUpdateType() const
    {
        return static_cast<VersionType>(get("PreferredUpdateType", static_cast<int>(VersionType::Stable)));
    }

    void Configuration::setPreferredUpdateType(VersionType type)
    {
        set("PreferredUpdateType", static_cast<int>(type));
    }
}
