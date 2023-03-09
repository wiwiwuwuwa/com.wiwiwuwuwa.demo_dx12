//#include <pch.h>
//#include <aiva2/render_texture_2d.hpp>
//
//#include <aiva2/assert.hpp>
//#include <aiva2/render_color_buffer_2d.hpp>
//#include <aiva2/render_depth_buffer_2d.hpp>
//
//namespace aiva2
//{
//	render_target_t::render_target_t(engine_t& engine)
//		: impl_type{ engine }
//	{
//		
//	}
//
//	render_target_t::~render_target_t()
//	{
//		
//	}
//
//	auto render_target_t::get_color_cpu_descriptor_handles() const->std::vector<D3D12_CPU_DESCRIPTOR_HANDLE>
//	{
//		auto handles = std::vector<D3D12_CPU_DESCRIPTOR_HANDLE>{};
//		for (auto i = size_t{}; i < std::size(m_color_buffers); i++)
//		{
//			auto const& buffer = m_color_buffers[i];
//			assert_t::check_bool(buffer, "buffer is not valid");
//
//			auto const handle = (*buffer).get_cpu_descriptor_handle();
//			handles.push_back(handle);
//		}
//		return handles;
//	}
//
//	auto render_target_t::get_depth_cpu_descriptor_handle() const->std::optional<D3D12_CPU_DESCRIPTOR_HANDLE>
//	{
//		auto const& buffer = m_depth_buffer;
//		if (!buffer)
//		{
//			return {};
//		}
//
//		return (*buffer).get_cpu_descriptor_handle();
//	}
//
//	void render_target_t::set_state_for_transition() const
//	{
//		for (auto const& color_buffer : m_color_buffers)
//		{
//			assert_t::check_bool(color_buffer, "color_buffer is not valid");
//			(*color_buffer).set_state_for_transition();
//		}
//
//		if (m_depth_buffer)
//		{
//			(*m_depth_buffer).set_state_for_transition();
//		}
//	}
//
//	void render_target_t::set_state_for_uav() const
//	{
//		for (auto const& color_buffer : m_color_buffers)
//		{
//			assert_t::check_bool(color_buffer, "color_buffer is not valid");
//			(*color_buffer).set_state_for_uav();
//		}
//
//		if (m_depth_buffer)
//		{
//			(*m_depth_buffer).set_state_for_uav();
//		}
//	}
//
//	void render_target_t::add_color_buffer(std::shared_ptr<render_color_buffer_2d_t> const& buffer)
//	{
//		assert_t::check_bool(buffer, "buffer is not valid");
//		m_color_buffers.push_back(buffer);
//	}
//
//	auto render_target_t::get_color_buffer(size_t const index) const->std::shared_ptr<render_color_buffer_2d_t> const&
//	{
//		assert_t::check_bool(index >= 0 && index < std::size(m_color_buffers), "index is out of range");
//		return m_color_buffers[index];
//	}
//
//	void render_target_t::rem_color_buffer(size_t const index)
//	{
//		assert_t::check_bool(index >= 0 && index < std::size(m_color_buffers), "index is out of range");
//		m_color_buffers.erase(std::next(std::cbegin(m_color_buffers), index));
//	}
//	
//	auto render_target_t::num_color_buffers() const->size_t
//	{
//		return std::size(m_color_buffers);
//	}
//
//	auto render_target_t::get_depth_buffer() const->std::shared_ptr<render_depth_buffer_2d_t> const&
//	{
//		return m_depth_buffer;
//	}
//
//	auto render_target_t::has_depth_buffer() const->bool
//	{
//		return m_depth_buffer != nullptr;
//	}
//
//	void render_target_t::set_depth_buffer(std::shared_ptr<render_depth_buffer_2d_t> const& buffer)
//	{
//		m_depth_buffer = buffer;
//	}
//}


#include <pch.h>
#include <aiva2/render_target.hpp>

#include <aiva2/assert.hpp>
#include <aiva2/render_color_target.hpp>
#include <aiva2/render_depth_target.hpp>

namespace aiva2
{
	render_target_t::render_target_t(engine_t& engine)
		: impl_type{ engine }
	{

	}

	render_target_t::~render_target_t()
	{

	}

	void render_target_t::set_state_for_transition() const
	{
		for (auto const& color_target : m_color_targets)
		{
			assert_t::check_bool(color_target, "color_target is not valid");
			(*color_target).set_state_for_transition();
		}

		if (m_depth_target)
		{
			(*m_depth_target).set_state_for_transition();
		}
	}

	void render_target_t::set_state_for_uav() const
	{
		for (auto const& color_target : m_color_targets)
		{
			assert_t::check_bool(color_target, "color_target is not valid");
			(*color_target).set_state_for_uav();
		}

		if (m_depth_target)
		{
			(*m_depth_target).set_state_for_uav();
		}
	}

	void render_target_t::add_color_target(std::shared_ptr<render_color_target_t> const& color_target)
	{
		assert_t::check_bool(color_target, "color_target is not valid");
		m_color_targets.push_back(color_target);
	}

	auto render_target_t::get_color_target(size_t const index) const->std::shared_ptr<render_color_target_t> const&
	{
		assert_t::check_bool(index >= 0 && index < std::size(m_color_targets), "index is out of range");
		return m_color_targets[index];
	}

	auto render_target_t::num_color_target() const->size_t
	{
		return std::size(m_color_targets);
	}

	void render_target_t::rem_color_target(size_t const index)
	{
		assert_t::check_bool(index >= 0 && index < std::size(m_color_targets), "index is out of range");
		m_color_targets.erase(std::next(std::cbegin(m_color_targets), index));
	}
	
	void render_target_t::set_color_target(size_t const index, std::shared_ptr<render_color_target_t> const& color_target)
	{
		assert_t::check_bool(index >= 0 && index < std::size(m_color_targets), "index is out of range");
		assert_t::check_bool(color_target, "color_target is not valid");
		m_color_targets[index] = color_target;
	}

	auto render_target_t::get_color_cpu_descriptor_handles() const->std::vector<D3D12_CPU_DESCRIPTOR_HANDLE>
	{
		auto handles = std::vector<D3D12_CPU_DESCRIPTOR_HANDLE>{};
		for (auto const& color_target : m_color_targets)
		{
			assert_t::check_bool(color_target, "color_target is not valid");
			handles.push_back((*color_target).get_cpu_descriptor_handle());
		}
		return handles;
	}

	auto render_target_t::get_depth_target() const->std::shared_ptr<render_depth_target_t> const&
	{
		return m_depth_target;
	}

	auto render_target_t::has_depth_target() const->bool
	{
		return m_depth_target != nullptr;
	}

	void render_target_t::set_depth_target(std::shared_ptr<render_depth_target_t> const& depth_target)
	{
		m_depth_target = depth_target;
	}

	auto render_target_t::get_depth_cpu_descriptor_handle() const->std::optional<D3D12_CPU_DESCRIPTOR_HANDLE>
	{
		if (m_depth_target)
		{
			return (*m_depth_target).get_cpu_descriptor_handle();
		}
		else
		{
			return {};
		}
	}
}
