#include "helpers/host.h"
#include "controllers/main_window_controller.h"
#include "services/app_config_service.h"
#include "services/folder_service.h"

using namespace application::controllers;
using namespace application::services;
using namespace desktop::app;
using namespace desktop::services;

namespace application::helpers
{
	desktop::hosting::host host::create_host(std::span<char*> args)
	{
		std::shared_ptr<app_info> info{ std::make_shared<app_info>("org.nickvision.application", "Application", "Application", true) };
		info->set_version({ 2026, 6, 0 });
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
		h.get_services()->add<folder_service>(service_scope::singleton);
		h.get_services()->add<main_window_controller>(service_scope::singleton);
		h.use_github_updates();
		return h;
	}
}