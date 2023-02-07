#pragma once
#include <aiva2/base.hpp>

#include <aiva2/core/implements.hpp>
#include <aiva2/core/struct.hpp>
#include <aiva2/native/buffer_format.hpp>

namespace aiva2::native
{
	struct gr_texture_2d_params_t final : public core::implements_t<gr_texture_2d_params_t, core::struct_t>
	{
		// ------------------------------------------------
		// texture 2d params

	public:
		auto get_format() const->buffer_format_t;

		void set_format(buffer_format_t const format);

		auto get_width() const->size_t;

		void set_width(size_t const width);

		auto get_height() const->size_t;

		void set_height(size_t const height);
		
		auto get_support_mip_mappping() const->bool;

		void set_support_mip_mappping(bool const support_mip_mappping);

		auto get_support_shader_resource() const->bool;

		void set_support_shader_resource(bool const support_shader_resource);

		auto get_support_shader_atomics() const->bool;

		void set_support_shader_atomics(bool const support_shader_atomics);

		auto get_support_render_target() const->bool;

		void set_support_render_target(bool const support_render_target);

		auto get_support_depth_stencil() const->bool;

		void set_support_depth_stencil(bool const support_depth_stencil);

		auto get_support_unordered_access() const->bool;

		void set_support_unordered_access(bool const support_unordered_access);

	private:
		buffer_format_t m_format{};

		size_t m_width{};

		size_t m_height{};

		bool m_support_mip_mappping{};

		bool m_support_shader_resource{};
		
		bool m_support_shader_atomics{};

		bool m_support_render_target{};

		bool m_support_depth_stencil{};

		bool m_support_unordered_access{};
	};
}
