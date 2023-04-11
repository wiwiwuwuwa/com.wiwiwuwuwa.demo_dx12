#include <pch.h>
#include <aiva2/shader_info_for_resource_utils.hpp>

#include <aiva2/assert.hpp>
#include <aiva2/shader_info_for_resource.hpp>

namespace aiva2
{
	auto operator<(shader_info_for_resource_t const& lhs, shader_info_for_resource_t const& rhs)->bool
	{
		if (lhs.get_frequency() != rhs.get_frequency())
		{
			return lhs.get_frequency() < rhs.get_frequency();
		}

		if (lhs.get_register_type() != rhs.get_register_type())
		{
			return lhs.get_register_type() < rhs.get_register_type();
		}

		if (lhs.get_register_space() != rhs.get_register_space())
		{
			return lhs.get_register_space() < rhs.get_register_space();
		}

		if (lhs.get_register_index() != rhs.get_register_index())
		{
			return lhs.get_register_index() < rhs.get_register_index();
		}

		return false;
	}

	auto operator<(std::shared_ptr<shader_info_for_resource_t> const& lhs, std::shared_ptr<shader_info_for_resource_t> const& rhs)->bool
	{
		assert_t::check_bool(lhs, "lhs is not valid");
		assert_t::check_bool(rhs, "rhs is not valid");

		return (*lhs) < (*rhs);
	}
}
