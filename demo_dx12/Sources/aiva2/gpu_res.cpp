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

	auto gpu_res_t::get_gpu_virtual_address() const->D3D12_GPU_VIRTUAL_ADDRESS
	{
		auto const resource = get_resource();
		assert_t::check_bool(resource, "(resource) is not valid");

		return (*resource).GetGPUVirtualAddress();
	}

	auto gpu_res_t::set_state(D3D12_RESOURCE_STATES const state, std::optional<size_t> const index /*= {}*/) ->std::vector<D3D12_RESOURCE_BARRIER>
	{
		assert_t::check_bool(m_resource, "m_resource is not valid");

		auto barriers = set_state_for_transition(state, index);
		if (auto const uav_barrier = set_state_for_uav()) barriers.emplace_back(*uav_barrier);
		
		return barriers;
	}

	auto gpu_res_t::set_state_for_transition(D3D12_RESOURCE_STATES const state, std::optional<size_t> const index /*= {}*/) -> std::vector<D3D12_RESOURCE_BARRIER>
	{
		assert_t::check_bool(m_resource, "m_resource is not valid");

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

	auto gpu_res_t::set_state_for_uav() ->std::optional<D3D12_RESOURCE_BARRIER>
	{
		assert_t::check_bool(m_resource, "m_resource is not valid");
		
		auto barrier = std::optional<D3D12_RESOURCE_BARRIER>{};

		if (!d3d12_resource_utils_t::get_support_unordered_access(*m_resource))
		{
			return barrier;
		}
		
		barrier = D3D12_RESOURCE_BARRIER{};
		(*barrier).Type = D3D12_RESOURCE_BARRIER_TYPE_UAV;
		(*barrier).Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		(*barrier).UAV.pResource = m_resource.get();
		
		return barrier;
	}
}
