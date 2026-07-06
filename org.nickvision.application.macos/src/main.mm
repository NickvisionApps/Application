#include <libdesktop.h>
#import "app_delegate.h"
#include "cocoa_lifetime_service.h"
#import "controls/main_window_content.h"
#import "controls/main_window_sidebar.h"
#import "controls/main_window_split_view.h"
#include "helpers/host.h"
#import "views/main_window.h"
#import "views/settings_dialog.h"

using namespace application::helpers::host;
using namespace application::macos;
using namespace application::macos::controls;
using namespace application::macos::views;
using namespace desktop::hosting;
using namespace desktop::services;

int main(int argc, char* argv[])
{
	host h{ create_host({ argv, static_cast<size_t>(argc) }) };
	h.use_lifetime<cocoa_lifetime_service>();
	h.get_services()->add<main_window>(service_scope::singleton);
	h.get_services()->add<main_window_split_view>(service_scope::singleton);
	h.get_services()->add<main_window_sidebar>(service_scope::singleton);
	h.get_services()->add<main_window_content>(service_scope::singleton);
	h.get_services()->add<settings_dialog>(service_scope::latched);
	h.get_services()->add<app_delegate>(service_scope::singleton);
	std::exception_ptr ptr{ h.run() };
	if (ptr)
	{
		std::rethrow_exception(ptr);
	}
	return ptr ? 1 : 0;
}
