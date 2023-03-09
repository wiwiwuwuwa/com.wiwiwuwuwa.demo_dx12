#include <pch.h>
#include <aiva2/render_texture_2d.hpp>

#include <aiva2/assert.hpp>
#include <aiva2/render_color_buffer_2d.hpp>
#include <aiva2/render_depth_buffer_2d.hpp>

namespace aiva2
{
	render_texture_2d_t::render_texture_2d_t(engine_t& engine)
		: impl_type{ engine }
	{
		
	}

	render_texture_2d_t::~render_texture_2d_t()
	{
		
	}

	auto render_texture_2d_t::get_color_cpu_descriptor_handles() const->std::vector<D3D12_CPU_DESCRIPTOR_HANDLE>
	{
		auto handles = std::vector<D3D12_CPU_DESCRIPTOR_HANDLE>{};
		for (auto i = size_t{}; i < std::size(m_color_buffers); i++)
		{
			auto const& buffer = m_color_buffers[i];
			assert_t::check_bool(buffer, "buffer is not valid");

			auto const handle = (*buffer).get_cpu_descriptor_handle();
			handles.push_back(handle);
		}
		return handles;
	}

	auto render_texture_2d_t::get_depth_cpu_descriptor_handle() const->std::optional<D3D12_CPU_DESCRIPTOR_HANDLE>
	{
		auto const& buffer = m_depth_buffer;
		if (!buffer)
		{
			return {};
		}

		return (*buffer).get_cpu_descriptor_handle();
	}

	void render_texture_2d_t::set_state_for_transition() const
	{
		for (auto const& color_buffer : m_color_buffers)
		{
			assert_t::check_bool(color_buffer, "color_buffer is not valid");
			(*color_buffer).set_state_for_transition();
		}

		if (m_depth_buffer)
		{
			(*m_depth_buffer).set_state_for_transition();
		}
	}

	void render_texture_2d_t::set_state_for_uav() const
	{
		for (auto const& color_buffer : m_color_buffers)
		{
			assert_t::check_bool(color_buffer, "color_buffer is not valid");
			(*color_buffer).set_state_for_uav();
		}

		if (m_depth_buffer)
		{
			(*m_depth_buffer).set_state_for_uav();
		}
	}

	void render_texture_2d_t::add_color_buffer(std::shared_ptr<render_color_buffer_2d_t> const& buffer)
	{
		assert_t::check_bool(buffer, "buffer is not valid");
		m_color_buffers.push_back(buffer);
	}

	auto render_texture_2d_t::get_color_buffer(size_t const index) const->std::shared_ptr<render_color_buffer_2d_t> const&
	{
		assert_t::check_bool(index >= 0 && index < std::size(m_color_buffers), "index is out of range");
		return m_color_buffers[index];
	}

	void render_texture_2d_t::rem_color_buffer(size_t const index)
	{
		assert_t::check_bool(index >= 0 && index < std::size(m_color_buffers), "index is out of range");
		m_color_buffers.erase(std::next(std::cbegin(m_color_buffers), index));
	}
	
	auto render_texture_2d_t::num_color_buffers() const->size_t
	{
		return std::size(m_color_buffers);
	}

	auto render_texture_2d_t::get_depth_buffer() const->std::shared_ptr<render_depth_buffer_2d_t> const&
	{
		return m_depth_buffer;
	}

	auto render_texture_2d_t::has_depth_buffer() const->bool
	{
		return m_depth_buffer != nullptr;
	}

	void render_texture_2d_t::set_depth_buffer(std::shared_ptr<render_depth_buffer_2d_t> const& buffer)
	{
		m_depth_buffer = buffer;
	}
}
