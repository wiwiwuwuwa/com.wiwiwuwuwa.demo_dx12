#include <pch.h>
#include <aiva2/d3d12_resource_desc_utils.hpp>

namespace aiva2
{
	size_t d3d12_resource_desc_utils_t::get_array_size(D3D12_RESOURCE_DESC const& desc)
	{
		return (desc.Dimension != D3D12_RESOURCE_DIMENSION_TEXTURE3D) ? static_cast<size_t>(desc.DepthOrArraySize) : size_t{ 1 };
	}

	size_t d3d12_resource_desc_utils_t::get_subresource_index(D3D12_RESOURCE_DESC const& desc, size_t const mip_slice, size_t const array_slice, size_t const plane_slice)
	{
		auto const array_size = get_array_size(desc);
		auto const mip_levels = static_cast<size_t>(desc.MipLevels);
		return mip_slice + (array_slice * mip_levels) + (plane_slice * mip_levels * array_size);
	}

	size_t d3d12_resource_desc_utils_t::get_subresources_count(D3D12_RESOURCE_DESC const& desc)
	{
		auto const array_size = get_array_size(desc);
		auto const mip_levels = static_cast<size_t>(desc.MipLevels);
		return mip_levels * array_size;
	}
}
