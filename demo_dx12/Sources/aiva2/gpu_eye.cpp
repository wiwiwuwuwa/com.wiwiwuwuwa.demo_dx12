#include <pch.h>
#include <aiva2/gpu_eye.hpp>

#include <aiva2/engine.hpp>

namespace aiva2
{
	gpu_eye_t::gpu_eye_t(engine_t& engine)
		: impl_type{ engine }
	{
		
	}

	gpu_eye_t::~gpu_eye_t()
	{

	}
}
