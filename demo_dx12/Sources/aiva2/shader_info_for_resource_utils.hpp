#pragma once
#include <aiva2/_core.hpp>

namespace aiva2
{
	auto operator<(shader_info_for_resource_t const& lhs, shader_info_for_resource_t const& rhs)->bool;
	
	auto operator<(std::shared_ptr<shader_info_for_resource_t> const& lhs, std::shared_ptr<shader_info_for_resource_t> const& rhs)->bool;
}
