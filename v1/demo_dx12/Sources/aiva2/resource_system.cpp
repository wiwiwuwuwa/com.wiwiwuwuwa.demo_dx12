#include <pch.h>
#include <aiva2/resource_system.hpp>

#include <aiva2/assert.hpp>
#include <aiva2/engine.hpp>
#include <aiva2/string_view_utils.hpp>

namespace aiva2
{
	resource_system_t::resource_system_t(engine_t& engine)
		: impl_type{ engine }
	{
		
	}

	resource_system_t::~resource_system_t()
	{
		
	}
}
