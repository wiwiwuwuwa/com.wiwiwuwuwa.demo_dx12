#pragma once
#include <aiva2/_core.hpp>

namespace aiva2
{
	enum class shader_register_type_t
	{
		UNKNOWN,
		CONSTANT_BUFFER,
		SHADER_RESOURCE,
		UNORDERED_ACCESS,
		SAMPLER,
		MAXIMUM,
	};
}
