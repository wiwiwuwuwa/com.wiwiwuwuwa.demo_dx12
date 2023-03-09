#include <pch.h>
#include <aiva2/render_buffer_2d_base.hpp>

#include <aiva2/assert.hpp>
#include <aiva2/engine.hpp>
#include <aiva2/texture_2d.hpp>

namespace aiva2
{
	render_buffer_2d_base_t::render_buffer_2d_base_t(engine_t& engine)
		: impl_type{ engine }
	{
		
	}

	render_buffer_2d_base_t::~render_buffer_2d_base_t()
	{
		
	}

	auto render_buffer_2d_base_t::get_resource() const->std::shared_ptr<texture_2d_t> const&
	{
		return m_resource;
	}

	auto render_buffer_2d_base_t::get_cpu_descriptor_handle() const->D3D12_CPU_DESCRIPTOR_HANDLE
	{
		assert_t::check_bool(m_descriptor_heap, "m_descriptor_heap is not valid");
		return (*m_descriptor_heap).GetCPUDescriptorHandleForHeapStart();
	}
}
