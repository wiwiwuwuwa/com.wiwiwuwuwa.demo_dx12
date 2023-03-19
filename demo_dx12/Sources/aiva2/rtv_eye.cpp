#include <pch.h>
#include <aiva2/rtv_eye.hpp>

#include <aiva2/engine.hpp>

namespace aiva2
{
	rtv_eye_t::rtv_eye_t(engine_t& engine)
		: impl_type{ engine }
	{

	}

	rtv_eye_t::~rtv_eye_t()
	{

	}
}
