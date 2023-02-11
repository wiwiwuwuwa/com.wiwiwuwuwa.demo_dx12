#include <pch.h>
#include <aiva2/native/d3d12_resource_desc_utils.hpp>

namespace aiva2::native
{
	size_t d3d12_resource_desc_utils_t::get_subresources_count(D3D12_RESOURCE_DESC const& desc)
	{
		auto const array_size = (desc.Dimension != D3D12_RESOURCE_DIMENSION_TEXTURE3D ? static_cast<size_t>(desc.DepthOrArraySize) : size_t{ 1 });
		auto const mip_levels = static_cast<size_t>(desc.MipLevels);
		return mip_levels * array_size;		
	}
}
