#include <libdesktop.h>
#include "cocoa_lifetime_service.h"
#include "helpers/host.h"

using namespace application::helpers::host;
using namespace application::macos;
using namespace desktop::hosting;

int main(int argc, char* argv[])
{
	host h{ create_host({ argv, static_cast<size_t>(argc) }) };
	h.use_lifetime<cocoa_lifetime_service>();
	return h.run() ? 1 : 0;
}
