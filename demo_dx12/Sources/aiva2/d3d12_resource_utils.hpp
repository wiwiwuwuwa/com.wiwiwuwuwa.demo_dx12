#pragma once
#include <aiva2/_core.hpp>

namespace aiva2
{
	struct d3d12_resource_utils_t final
	{
		// ------------------------------------------------

	private:
		d3d12_resource_utils_t() = delete;

	public:
		static size_t get_subresources_count(ID3D12Resource& resource);

		static size_t get_subresource_index(ID3D12Resource& resource, size_t const mip_slice, size_t const array_slice, size_t const plane_slice);

		// ------------------------------------------------
	};
}
