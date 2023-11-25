#pragma once
#include <aiva2/_core.hpp>

#include <aiva2/buffer_format.hpp>
#include <aiva2/gpu_eye_info.hpp>

namespace aiva2
{
	struct uav_tex_2d_info_t final : public implements_t<uav_tex_2d_info_t, gpu_eye_info_t>
	{
		// ------------------------------------------------

	public:
		uav_tex_2d_info_t() = default;

		~uav_tex_2d_info_t() override = default;

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