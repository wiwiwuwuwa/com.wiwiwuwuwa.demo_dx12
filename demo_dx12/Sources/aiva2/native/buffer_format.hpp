#pragma once
#include <aiva2/base.hpp>

namespace aiva2::native
{
	enum class buffer_format_t
	{
		UNKNOWN,
		R8G8B8A8_UNORM,
		R32G32B32A32_FLOAT,
		D32_FLOAT,
	};
}
