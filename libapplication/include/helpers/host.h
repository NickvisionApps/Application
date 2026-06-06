#pragma once

#include <libdesktop.h>
#include <span>

namespace application::helpers::host
{
	desktop::hosting::host create_host(std::span<char*> args);
}