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
		static size_t get_subresource_index(tex_2d_t const& resource, size_t const mip_slice, size_t const plane_slice);

		static size_t get_subresource_index(tex_2d_t const& resource, render_color_texture_2d_info_t const& info);

		static size_t get_subresource_index(tex_2d_t const& resource, render_depth_texture_2d_info_t const& info);
		
		// ------------------------------------------------
	};
}
