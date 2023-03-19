#include <pch.h>
#include <aiva2/dsv_eye.hpp>

#include <aiva2/engine.hpp>

namespace aiva2
{
	dsv_eye_t::dsv_eye_t(engine_t& engine)
		: impl_type{ engine }
	{

	}

	dsv_eye_t::~dsv_eye_t()
	{

	}
}
