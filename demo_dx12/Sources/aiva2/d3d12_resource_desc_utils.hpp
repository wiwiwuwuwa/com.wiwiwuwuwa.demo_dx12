#pragma once
#include <aiva2/_core.hpp>

namespace aiva2
{
	struct d3d12_resource_desc_utils_t final
	{
		// ------------------------------------------------

	private:
		d3d12_resource_desc_utils_t() = delete;

	public:
		static size_t get_array_size(D3D12_RESOURCE_DESC const& desc);
		
		static size_t get_subresource_index(D3D12_RESOURCE_DESC const& desc, size_t const mip_slice, size_t const array_slice, size_t const plane_slice);
		
		static size_t get_subresources_count(D3D12_RESOURCE_DESC const& desc);
		
		// ------------------------------------------------
	};
}
