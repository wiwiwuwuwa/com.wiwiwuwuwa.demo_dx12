#include <pch.h>
#include <aiva2/render_color_texture_2d_info.hpp>

namespace aiva2
{
	auto render_color_texture_2d_info_t::get_format() const->buffer_format_t
	{
		return m_format;
	}

	void render_color_texture_2d_info_t::set_format(buffer_format_t const format)
	{
		m_format = format;
	}

	auto render_color_texture_2d_info_t::get_mip_slice() const->size_t
	{
		return m_mip_slice;
	}

	void render_color_texture_2d_info_t::set_mip_slice(size_t const mip_slice)
	{
		m_mip_slice = mip_slice;
	}

	auto render_color_texture_2d_info_t::get_plane_slice() const->size_t
	{
		return m_plane_slice;
	}

	void render_color_texture_2d_info_t::set_plane_slice(size_t const plane_slice)
	{
		m_plane_slice = plane_slice;
	}
}
