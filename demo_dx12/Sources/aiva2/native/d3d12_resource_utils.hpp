#pragma once
#include <aiva2/base.hpp>

namespace aiva2::native
{
	struct d3d12_resource_utils_t final
	{
		// ------------------------------------------------
		// d3d12 resource utils

	private:
		d3d12_resource_utils_t() = delete;

	public:
		static size_t get_subresources_count(ID3D12Resource& resource);
	};
}
