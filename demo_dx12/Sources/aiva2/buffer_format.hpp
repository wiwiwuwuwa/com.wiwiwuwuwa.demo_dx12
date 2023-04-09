#pragma once
#include <aiva2/_core.hpp>

namespace aiva2
{
	enum class buffer_format_t
	{
		UNKNOWN,
		R32_SINT,
		R32_FLOAT,
		R32G32_FLOAT,
		R32G32B32_FLOAT,
		R32G32B32A32_FLOAT,
		R32_UINT,
		R8G8B8A8_UNORM,
		D32_FLOAT,
		MAXIMUM,
	};
}
