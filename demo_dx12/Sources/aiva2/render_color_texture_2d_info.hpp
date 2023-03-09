#pragma once
#include <aiva2/_core.hpp>

#include <aiva2/buffer_format.hpp>
#include <aiva2/implements.hpp>
#include <aiva2/struct.hpp>

namespace aiva2
{
	struct render_color_texture_2d_info_t final : public implements_t<render_color_texture_2d_info_t, struct_t>
	{
		// ------------------------------------------------

	public:
		auto get_format() const->buffer_format_t;

		void set_format(buffer_format_t const format);

	private:
		buffer_format_t m_format{ buffer_format_t::UNKNOWN };

		// ------------------------------------------------

	public:
		auto get_mip_slice() const->size_t;

		void set_mip_slice(size_t const mip_slice);

	private:
		size_t m_mip_slice{};

		// ------------------------------------------------

	public:
		auto get_plane_slice() const->size_t;

		void set_plane_slice(size_t const plane_slice);

	private:
		size_t m_plane_slice{};

		// ------------------------------------------------
	};
}