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
		static size_t get_subresources_count(D3D12_RESOURCE_DESC const& desc);
		
		// ------------------------------------------------
	};
}
