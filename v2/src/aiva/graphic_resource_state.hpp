#pragma once
#include <optional>
#include <vector>
#include <d3d12.h>
#include <aiva/implements.hpp>

namespace aiva
{
    struct graphic_resource_state_t final : public implements_t<graphic_resource_state_t>
    {
        // ------------------------------------------------

    public:
        graphic_resource_state_t();

        graphic_resource_state_t(D3D12_RESOURCE_STATES const state, size_t const count);

    public:
        auto set_state(D3D12_RESOURCE_STATES const state, std::optional<size_t> const index = {}) -> std::vector<D3D12_RESOURCE_TRANSITION_BARRIER>;

    private:
        auto set_state_vec(D3D12_RESOURCE_STATES const state) -> std::vector<D3D12_RESOURCE_TRANSITION_BARRIER>;

        auto set_state_vec(D3D12_RESOURCE_STATES const state, size_t const index) -> std::vector<D3D12_RESOURCE_TRANSITION_BARRIER>;

        auto set_state_opt(D3D12_RESOURCE_STATES const state, size_t const index) -> std::optional<D3D12_RESOURCE_TRANSITION_BARRIER>;

    private:
        std::vector<D3D12_RESOURCE_STATES> m_states{};

        // ------------------------------------------------
    };
} // namespace aiva
