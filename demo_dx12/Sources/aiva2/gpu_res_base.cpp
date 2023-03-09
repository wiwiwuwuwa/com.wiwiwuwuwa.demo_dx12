#include <pch.h>
#include <aiva2/gpu_res_base.hpp>

#include <aiva2/assert.hpp>
#include <aiva2/engine.hpp>

namespace aiva2
{
	gpu_res_base_t::gpu_res_base_t(engine_t& engine)
		: impl_type{ engine }
	{
		
	}

	gpu_res_base_t::~gpu_res_base_t()
	{
		
	}

	auto gpu_res_base_t::get_resource() const->winrt::com_ptr<ID3D12Resource> const&
	{
		return m_resource;
	}

	auto gpu_res_base_t::set_state_for_transition(D3D12_RESOURCE_STATES const state, std::optional<size_t> const index /*= {}*/) -> std::vector<D3D12_RESOURCE_BARRIER>
	{
		assert_t::check_bool(m_resource, "resource is not valid");
		
		auto barriers = std::vector<D3D12_RESOURCE_BARRIER>{};

		if (m_states.get_locked())
		{
			return barriers;
		}

		for (auto const& transition_barrier : m_states.set_state(state, index))
		{
			auto& barrier = barriers.emplace_back();
			barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
			barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
			barrier.Transition = transition_barrier;
			barrier.Transition.pResource = m_resource.get();
		}

		return barriers;
	}

	auto gpu_res_base_t::set_state_for_uav() -> D3D12_RESOURCE_BARRIER
	{
		assert_t::check_bool(m_resource, "resource is not valid");

		auto barrier = D3D12_RESOURCE_BARRIER{};
		barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_UAV;
		barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		barrier.UAV.pResource = m_resource.get();

		return barrier;
	}
}
