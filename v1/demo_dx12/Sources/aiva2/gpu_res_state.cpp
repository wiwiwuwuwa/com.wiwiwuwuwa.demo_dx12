#include <pch.h>
#include <aiva2/gpu_res_state.hpp>

#include <aiva2/assert.hpp>

namespace aiva2
{
	gpu_res_state_t::gpu_res_state_t() : m_states{}, m_locked{}
	{

	}

	gpu_res_state_t::gpu_res_state_t(D3D12_RESOURCE_STATES const state, size_t const count, bool const locked) : m_states{ count, state }, m_locked{ locked }
	{

	}

	gpu_res_state_t::~gpu_res_state_t()
	{

	}

	auto gpu_res_state_t::get_locked() const -> bool
	{
		return m_locked;
	}

	auto gpu_res_state_t::set_state(D3D12_RESOURCE_STATES const state, std::optional<size_t> const index) -> std::vector<D3D12_RESOURCE_TRANSITION_BARRIER>
	{
		assert_t::check_bool(std::size(m_states) >= 1, "resource has no subresources");
		assert_t::check_bool(!m_locked, "resource is locked");

		if (index.has_value())
		{
			return set_state_for_one_subresource_vec(state, index.value());
		}
		else
		{
			return set_state_for_all_subresources_vec(state);
		}
	}

	auto gpu_res_state_t::set_state_for_one_subresource_vec(D3D12_RESOURCE_STATES const state, size_t const index) -> std::vector<D3D12_RESOURCE_TRANSITION_BARRIER>
	{
		auto const barrier = set_state_for_one_subresource_opt(state, index);
		if (barrier.has_value())
		{
			return { barrier.value() };
		}
		else
		{
			return {};
		}
	}

	auto gpu_res_state_t::set_state_for_one_subresource_opt(D3D12_RESOURCE_STATES const state, size_t const index) -> std::optional<D3D12_RESOURCE_TRANSITION_BARRIER>
	{
		assert_t::check_bool(index >= 0 && index < std::size(m_states), "index out of range");

		auto const old_state = m_states[index];
		auto const new_state = state;

		auto const state_changed = (old_state != new_state);
		if (!state_changed)
		{
			return {};
		}

		m_states[index] = new_state;

		auto barrier = D3D12_RESOURCE_TRANSITION_BARRIER{};
		barrier.pResource = {};
		barrier.Subresource = static_cast<UINT>(index);
		barrier.StateBefore = old_state;
		barrier.StateAfter = new_state;
		return barrier;
	}

	auto gpu_res_state_t::set_state_for_all_subresources_vec(D3D12_RESOURCE_STATES const state) -> std::vector<D3D12_RESOURCE_TRANSITION_BARRIER>
	{
		auto barriers = std::vector<D3D12_RESOURCE_TRANSITION_BARRIER>{};
		for (auto i = size_t{}; i < std::size(m_states); ++i)
		{
			auto const barrier = set_state_for_one_subresource_opt(state, i);
			if (barrier.has_value())
			{
				barriers.push_back(barrier.value());
			}
		}
		return barriers;
	}
}
