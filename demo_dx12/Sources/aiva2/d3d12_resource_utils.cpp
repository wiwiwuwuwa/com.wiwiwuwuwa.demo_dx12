#include <pch.h>
#include <aiva2/d3d12_resource_utils.hpp>

#include <aiva2/d3d12_resource_desc_utils.hpp>

namespace aiva2
{
	size_t d3d12_resource_utils_t::get_subresources_count(ID3D12Resource& resource)
	{
		return d3d12_resource_desc_utils_t::get_subresources_count(resource.GetDesc());
	}

	size_t d3d12_resource_utils_t::get_subresource_index(ID3D12Resource& resource, size_t const mip_slice, size_t const array_slice, size_t const plane_slice)
	{
		return d3d12_resource_desc_utils_t::get_subresource_index(resource.GetDesc(), mip_slice, array_slice, plane_slice);
	}

	auto d3d12_resource_utils_t::get_support_unordered_access(ID3D12Resource& resource) -> bool
	{
		return d3d12_resource_desc_utils_t::get_support_unordered_access(resource.GetDesc());
	}
}
