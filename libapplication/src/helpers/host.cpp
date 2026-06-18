#include "helpers/host.h"
#include "controllers/main_window_controller.h"
#include "controllers/preferences_view_controller.h"
#include "services/app_config_service.h"
#include "services/events_service.h"
#include "services/folder_service.h"

using namespace application::controllers;
using namespace application::services;
using namespace desktop::app;
using namespace desktop::services;

namespace application::helpers
{
	desktop::hosting::host host::create_host(std::span<char*> args)
	{
		std::shared_ptr<app_info> info{ std::make_shared<app_info>("org.nickvision.application", "Nickvision Application", "Application", true) };
		info->set_version({ 2026, 6, 0, "next" });
		info->set_changelog("- Initial release");
		info->set_source_url("https://github.com/NickvisionApps/Application");
		info->set_issues_url("https://github.com/NickvisionApps/Application/issues");
		info->set_discussions_url("https://github.com/NickvisionApps/Application/discussions");
#ifdef _WIN32
		bool contains_portable_flag{ false };
		for (char* arg : args)
		{
			if (strcmp(arg, "--portable") == 0)
			{
				contains_portable_flag = true;
				break;
			}
		}
		info->set_portable(contains_portable_flag);
#endif
		desktop::hosting::host h{ info, args };
		h.get_services()->add<app_config_service>(service_scope::singleton);
		h.get_services()->add<events_service>(service_scope::singleton);
		h.get_services()->add<folder_service>(service_scope::singleton);
		h.get_services()->add<main_window_controller>(service_scope::transient);
		h.get_services()->add<preferences_view_controller>(service_scope::transient);
		h.use_github_updates();
		std::shared_ptr<translation_service> translator{ h.get_services()->get_required<translation_service>() };
		info->set_short_name(translator->_("Application"));
		info->set_description(translator->_("Create new Nickvision application"));
		info->add_extra_link(translator->_("Matrix Chat"), "https://matrix.to/#/#nickvision:matrix.org");
		info->add_developer(translator->_("Nicholas Logozzo"), "https://github.com/nlogozzo");
		info->add_developer(translator->_("Contributors on GitHub ❤️"), "https://github.com/NickvisionApps/Application/graphs/contributors");
		info->add_designer(translator->_("Nicholas Logozzo"), "https://github.com/nlogozzo");
		info->add_designer(translator->_("Fyodor Sobolev"), "https://github.com/fsobolev");
		info->add_designer(translator->_("DaPigGuy"), "https://github.com/DaPigGuy");
		info->add_artist(translator->_("David Lapshin"), "https://github.com/daudix");
		info->set_translation_credits(translator->_("translation-credits"));
		return h;
	}
}