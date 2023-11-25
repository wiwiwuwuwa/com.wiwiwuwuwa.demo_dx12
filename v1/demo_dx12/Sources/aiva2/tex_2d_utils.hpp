#pragma once
#include <aiva2/_core.hpp>

namespace aiva2
{
	struct tex_2d_utils_t final
	{
		// ------------------------------------------------

	private:
		tex_2d_utils_t() = delete;

	public:
		static size_t get_subresource_index(tex_2d_t const& resource, size_t const mip_slice = {}, size_t const plane_slice = {});
		
		// ------------------------------------------------
	};
}
