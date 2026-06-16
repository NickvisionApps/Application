#include <libdesktop.h>
#include "gtk_lifetime_service.h"
#include "helpers/host.h"
#include "views/main_window.h"

using namespace application::helpers::host;
using namespace application::linux;
using namespace application::linux::views;
using namespace desktop::hosting;
using namespace desktop::services;

int main(int argc, char* argv[])
{
	host h{ create_host({ argv, static_cast<size_t>(argc) }) };
	h.use_lifetime<gtk_lifetime_service>();
	h.get_services()->add<main_window>(service_scope::singleton);
	std::exception_ptr ptr{ h.run() };
	if (ptr)
	{
		std::rethrow_exception(ptr);
	}
	return ptr ? 1 : 0;
}
