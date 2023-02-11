#pragma once
#include <aiva2/base.hpp>

namespace aiva2::native
{
	struct d3d12_resource_desc_utils_t final
	{
		// ------------------------------------------------
		// d3d12 resource desc utils

	private:
		d3d12_resource_desc_utils_t() = delete;

	public:
		static size_t get_subresources_count(D3D12_RESOURCE_DESC const& desc);
	};
}
