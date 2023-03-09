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
