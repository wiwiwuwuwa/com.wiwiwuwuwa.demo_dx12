#include <pch.h>
#include <aiva2/gpu_res.hpp>

#include <aiva2/assert.hpp>
#include <aiva2/d3d12_resource_utils.hpp>
#include <aiva2/engine.hpp>
#include <aiva2/graphic_hardware.hpp>

namespace aiva2
{
	gpu_res_t::gpu_res_t(engine_t& engine)
		: impl_type{ engine }
	{
		
	}

	gpu_res_t::~gpu_res_t()
	{
		
	}

	auto gpu_res_t::get_resource() const->winrt::com_ptr<ID3D12Resource> const&
	{
		return m_resource;
	}

	void gpu_res_t::init_state_for_transition(D3D12_RESOURCE_STATES const state, std::optional<size_t> const index /*= {}*/)
	{
		assert_t::check_bool(m_resource, "m_resource is not valid");
		
		if (m_states.get_locked())
		{
			return;
		}
		
		for (auto const& transition_barrier : m_states.set_state(state, index))
		{
			auto barrier = D3D12_RESOURCE_BARRIER{};
			barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
			barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
			barrier.Transition = transition_barrier;
			barrier.Transition.pResource = m_resource.get();

			get_engine().get_graphic_hardware().get_command_list().ResourceBarrier(1, &barrier);
		}
	}

	void gpu_res_t::init_state_for_uav()
	{
		assert_t::check_bool(m_resource, "m_resource is not valid");

		if (!d3d12_resource_utils_t::get_support_unordered_access(*m_resource))
		{
			return;
		}

		auto barrier = D3D12_RESOURCE_BARRIER{};
		barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_UAV;
		barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		barrier.UAV.pResource = m_resource.get();

		get_engine().get_graphic_hardware().get_command_list().ResourceBarrier(1, &barrier);
	}
}
