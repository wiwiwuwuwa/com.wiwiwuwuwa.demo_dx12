#include <pch.h>
#include <aiva2/gpu_cmd_base.hpp>

namespace aiva2
{
	gpu_cmd_base_t::gpu_cmd_base_t(engine_t& engine)
		: impl_type{ engine }
	{
		
	}

	gpu_cmd_base_t::~gpu_cmd_base_t()
	{
		
	}
}
