#pragma once
#include <aiva2/_core.hpp>

namespace aiva2
{
	template <typename t_enum>
	constexpr auto is_valid(t_enum const value)
	{
		if (value <= t_enum::UNKNOWN)
		{
			return false;
		}

		if (value >= t_enum::MAXIMUM)
		{
			return false;
		}

		return true;
	}
}
