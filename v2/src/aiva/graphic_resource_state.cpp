#include <aiva/graphic_resource_state.hpp>
#include <aiva/assert.hpp>

namespace aiva
{
    // ------------------------------------------------

    graphic_resource_state_t::graphic_resource_state_t()
    {

    }

    graphic_resource_state_t::graphic_resource_state_t(D3D12_RESOURCE_STATES const state, size_t const count)
        : m_states{ count, state }
    {

    }

    auto graphic_resource_state_t::set_state(D3D12_RESOURCE_STATES const state, std::optional<size_t> const index) -> std::vector<D3D12_RESOURCE_TRANSITION_BARRIER>
    {
        if (index)
        {
            return set_state_vec(state, index.value());
        }
        else
        {
            return set_state_vec(state);
        }
    }

    auto graphic_resource_state_t::set_state_vec(D3D12_RESOURCE_STATES const state) -> std::vector<D3D12_RESOURCE_TRANSITION_BARRIER>
    {
        auto barriers = std::vector<D3D12_RESOURCE_TRANSITION_BARRIER>{};

        for (auto i = size_t{}; i < std::size(m_states); i++)
        {
            if (auto const barrier = set_state_opt(state, i))
            {
                barriers.push_back(barrier.value());
            }
        }

        return barriers;
    }

    auto graphic_resource_state_t::set_state_vec(D3D12_RESOURCE_STATES const state, size_t const index) -> std::vector<D3D12_RESOURCE_TRANSITION_BARRIER>
    {
        if (auto const barrier = set_state_opt(state, index))
        {
            return { barrier.value() };
        }
        else
        {
            return {};
        }
    }

    auto graphic_resource_state_t::set_state_opt(D3D12_RESOURCE_STATES const state, size_t const index) -> std::optional<D3D12_RESOURCE_TRANSITION_BARRIER>
    {
        assert_t::check_bool(index >= decltype(index){} && index < std::size(m_states), "(index) is not valid");

        auto const old_state = m_states[index];
        auto const new_state = state;

        if (old_state == new_state)
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

    // ------------------------------------------------
} // namespace aiva
