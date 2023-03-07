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

		// ------------------------------------------------
	};
}
