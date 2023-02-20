#include <pch.h>
#include <aiva2/native/texture_2d_params.hpp>

namespace aiva2::native
{
	auto texture_2d_params_t::get_format() const->buffer_format_t
	{
		return m_format;
	}

	void texture_2d_params_t::set_format(buffer_format_t const format)
	{
		m_format = format;
	}

	auto texture_2d_params_t::get_width() const->size_t
	{
		return m_width;
	}

	void texture_2d_params_t::set_width(size_t const width)
	{
		m_width = width;
	}
	
	auto texture_2d_params_t::get_height() const->size_t
	{
		return m_height;
	}

	void texture_2d_params_t::set_height(size_t const height)
	{
		m_height = height;
	}

	auto texture_2d_params_t::get_support_mip_mappping() const->bool
	{
		return m_support_mip_mappping;
	}

	void texture_2d_params_t::set_support_mip_mappping(bool const support_mip_mappping)
	{
		m_support_mip_mappping = support_mip_mappping;
	}

	auto texture_2d_params_t::get_support_shader_resource() const->bool
	{
		return m_support_shader_resource;
	}

	void texture_2d_params_t::set_support_shader_resource(bool const support_shader_resource)
	{
		m_support_shader_resource = support_shader_resource;
	}

	auto texture_2d_params_t::get_support_shader_atomics() const->bool
	{
		return m_support_shader_atomics;
	}

	void texture_2d_params_t::set_support_shader_atomics(bool const support_shader_atomics)
	{
		m_support_shader_atomics = support_shader_atomics;
	}

	auto texture_2d_params_t::get_support_render_target() const->bool
	{
		return m_support_render_target;
	}

	void texture_2d_params_t::set_support_render_target(bool const support_render_target)
	{
		m_support_render_target = support_render_target;
	}

	auto texture_2d_params_t::get_support_depth_stencil() const->bool
	{
		return m_support_depth_stencil;
	}

	void texture_2d_params_t::set_support_depth_stencil(bool const support_depth_stencil)
	{
		m_support_depth_stencil = support_depth_stencil;
	}
	
	auto texture_2d_params_t::get_support_unordered_access() const->bool
	{
		return m_support_unordered_access;
	}

	void texture_2d_params_t::set_support_unordered_access(bool const support_unordered_access)
	{
		m_support_unordered_access = support_unordered_access;
	}
}
