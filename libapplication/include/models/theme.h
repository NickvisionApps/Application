#pragma once

#include <cstdint>

namespace application::models
{
	enum class theme : std::uint8_t
	{
		light,
		dark,
		system
	};
}