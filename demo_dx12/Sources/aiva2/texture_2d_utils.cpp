#include <pch.h>
#include <aiva2/texture_2d_utils.hpp>

#include <aiva2/assert.hpp>
#include <aiva2/d3d12_resource_utils.hpp>
#include <aiva2/render_color_texture_2d_info.hpp>
#include <aiva2/texture_2d.hpp>

namespace aiva2
{
	size_t texture_2d_utils_t::get_subresource_index(texture_2d_t const& resource, size_t const mip_slice, size_t const plane_slice)
	{
		auto const& internal_resource = resource.get_resource();
		assert_t::check_bool(internal_resource, "internal_resource is not valid");

		return d3d12_resource_utils_t::get_subresource_index((*internal_resource), mip_slice, {}, plane_slice);
	}

	size_t texture_2d_utils_t::get_subresource_index(texture_2d_t const& resource, render_color_texture_2d_info_t const& info)
	{
		return get_subresource_index(resource, info.get_mip_slice(), info.get_plane_slice());
	}
}
