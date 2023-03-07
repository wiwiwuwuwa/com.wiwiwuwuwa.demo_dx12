#include <pch.h>
#include <aiva2/d3d12_resource_utils.hpp>

#include <aiva2/d3d12_resource_desc_utils.hpp>

namespace aiva2
{
	size_t d3d12_resource_utils_t::get_subresources_count(ID3D12Resource& resource)
	{
		auto const desc = resource.GetDesc();
		return d3d12_resource_desc_utils_t::get_subresources_count(desc);
	}
}
