#include <pch.h>
#include <aiva2/native/graphic_resource_states.hpp>

#include <aiva2/core/asserts.hpp>

namespace aiva2::native
{
	graphic_resource_states_t::graphic_resource_states_t() : m_states{}, m_locked{}
	{

	}

	graphic_resource_states_t::graphic_resource_states_t(D3D12_RESOURCE_STATES const state, size_t const count, bool const locked) : m_states{ count, state }, m_locked{ locked }
	{

	}

	graphic_resource_states_t::~graphic_resource_states_t()
	{

	}

	auto graphic_resource_states_t::set_state(D3D12_RESOURCE_STATES const state, std::optional<size_t> const index) -> std::vector<D3D12_RESOURCE_BARRIER>
	{
		core::asserts_t::check_true(std::size(m_states) >= 1, "resource has no subresources");
		core::asserts_t::check_true(!m_locked, "resource is locked");

		if (index.has_value())
		{
			return set_state_for_one_subresource_vec(state, index.value());
		}
		else
		{
			return set_state_for_all_subresources_vec(state);
		}
	}

	auto graphic_resource_states_t::set_state_for_one_subresource_vec(D3D12_RESOURCE_STATES const state, size_t const index) -> std::vector<D3D12_RESOURCE_BARRIER>
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

	auto graphic_resource_states_t::set_state_for_one_subresource_opt(D3D12_RESOURCE_STATES const state, size_t const index) -> std::optional<D3D12_RESOURCE_BARRIER>
	{
		core::asserts_t::check_true(index >= 0 && index < std::size(m_states), "index out of range");

		auto const old_state = m_states[index];
		auto const new_state = state;

		auto const state_changed = (old_state != new_state);
		if (!state_changed)
		{
			return {};
		}

		m_states[index] = new_state;

		auto barrier = D3D12_RESOURCE_BARRIER{};
		barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		barrier.Transition.pResource = {};
		barrier.Transition.Subresource = index;
		barrier.Transition.StateBefore = old_state;
		barrier.Transition.StateAfter = new_state;
		return barrier;
	}

	auto graphic_resource_states_t::set_state_for_all_subresources_vec(D3D12_RESOURCE_STATES const state) -> std::vector<D3D12_RESOURCE_BARRIER>
	{
		auto const are_all_states_equal = std::adjacent_find(std::cbegin(m_states), std::cend(m_states), std::not_equal_to{}) == std::end(m_states);
		if (are_all_states_equal)
		{
			return set_state_for_all_subresources_using_single_barrier_vec(state);
		}
		else
		{
			return set_state_for_all_subresources_using_multiple_barriers_vec(state);
		}
	}

	auto graphic_resource_states_t::set_state_for_all_subresources_using_single_barrier_vec(D3D12_RESOURCE_STATES const state) -> std::vector<D3D12_RESOURCE_BARRIER>
	{
		auto const barrier = set_state_for_all_subresources_using_single_barrier_opt(state);
		if (barrier.has_value())
		{
			return { barrier.value() };
		}
		else
		{
			return {};
		}
	}

	auto graphic_resource_states_t::set_state_for_all_subresources_using_single_barrier_opt(D3D12_RESOURCE_STATES const state) -> std::optional<D3D12_RESOURCE_BARRIER>
	{		
		core::asserts_t::check_true(std::size(m_states) > 0, "m_states is empty");

		auto const old_state = m_states[0];
		auto const new_state = state;
		
		auto const state_changed = (old_state != new_state);
		if (!state_changed)
		{
			return {};
		}
		
		std::fill(std::begin(m_states), std::end(m_states), new_state);
		
		auto barrier = D3D12_RESOURCE_BARRIER{};
		barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		barrier.Transition.pResource = {};
		barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
		barrier.Transition.StateBefore = old_state;
		barrier.Transition.StateAfter = new_state;
		return barrier;
	}
	
	auto graphic_resource_states_t::set_state_for_all_subresources_using_multiple_barriers_vec(D3D12_RESOURCE_STATES const state) -> std::vector<D3D12_RESOURCE_BARRIER>
	{
		auto barriers = std::vector<D3D12_RESOURCE_BARRIER>{};
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
