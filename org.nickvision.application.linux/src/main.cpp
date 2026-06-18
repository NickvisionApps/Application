#include <libdesktop.h>
#include "controls/shortcuts_dialog.h"
#include "gtk_lifetime_service.h"
#include "helpers/host.h"
#include "views/main_window.h"
#include "views/preferences_dialog.h"

using namespace application::helpers::host;
using namespace application::linux;
using namespace application::linux::controls;
using namespace application::linux::views;
using namespace desktop::hosting;
using namespace desktop::services;

int main(int argc, char* argv[])
{
	host h{ create_host({ argv, static_cast<size_t>(argc) }) };
	h.use_lifetime<gtk_lifetime_service>();
	h.get_services()->add<main_window>(service_scope::singleton);
	h.get_services()->add<preferences_dialog>(service_scope::latched);
	h.get_services()->add<shortcuts_dialog>(service_scope::latched);
	std::exception_ptr ptr{ h.run() };
	if (ptr)
	{
		std::rethrow_exception(ptr);
	}
	return ptr ? 1 : 0;
}
