#pragma once
#include <optional>
#include <vector>
#include <wrl/client.h>
#include <d3d12.h>
#include <aiva/assert.hpp>
#include <aiva/d3d12_resource_desc_utils.hpp>
#include <aiva/engine.hpp>
#include <aiva/engine_object.hpp>
#include <aiva/graphic_resource_state.hpp>
#include <aiva/i_support_unordered_access.hpp>

namespace aiva
{
    template <typename t_info>
    struct graphic_resource_t : public implements_t<graphic_resource_t<t_info>, engine_object_t>
    {
        // ------------------------------------------------

    public:
        graphic_resource_t(engine_t& engine, t_info const& info);

        graphic_resource_t(engine_t& engine, Microsoft::WRL::ComPtr<ID3D12Resource> const& resource);

    protected:
        virtual void create_resource_from_info(t_info const& in_info, Microsoft::WRL::ComPtr<ID3D12Resource>& out_resource, D3D12_RESOURCE_STATES& out_state) const = 0;

        virtual void create_info_from_resource(ID3D12Resource& in_resource, t_info& out_info, D3D12_RESOURCE_STATES& out_state) const = 0;

        // ------------------------------------------------

    public:
        auto get_resource_ptr() const -> Microsoft::WRL::ComPtr<ID3D12Resource> const&;

        auto get_resource_ref() const -> ID3D12Resource&;

        void set_resource(Microsoft::WRL::ComPtr<ID3D12Resource> const& resource);

    private:
        Microsoft::WRL::ComPtr<ID3D12Resource> m_resource{};

        // ------------------------------------------------

    public:
        auto get_info() const -> t_info const&;

        void set_info(t_info const& info);

    private:
        t_info m_info{};

        // ------------------------------------------------

    public:
        auto set_state(D3D12_RESOURCE_STATES const state, std::optional<size_t> const index = {}) -> std::vector<D3D12_RESOURCE_BARRIER>;

    private:
        graphic_resource_state_t m_states{};

        // ------------------------------------------------
    };
} // namespace aiva

// --------------------------------------------------------

namespace aiva
{
    template <typename t_info>
    graphic_resource_t<t_info>::graphic_resource_t(engine_t& engine, t_info const& info)
        : graphic_resource_t<t_info>::impl_type{ engine }
    {
        set_info(info);
    }

    template <typename t_info>
    graphic_resource_t<t_info>::graphic_resource_t(engine_t& engine, Microsoft::WRL::ComPtr<ID3D12Resource> const& resource)
        : graphic_resource_t<t_info>::impl_type{ engine }
    {
        set_resource(resource);
    }

    template <typename t_info>
    auto graphic_resource_t<t_info>::get_resource_ptr() const -> Microsoft::WRL::ComPtr<ID3D12Resource> const&
    {
        return m_resource;
    }

    template <typename t_info>
    auto graphic_resource_t<t_info>::get_resource_ref() const -> ID3D12Resource&
    {
        assert_t::check_bool(m_resource, "(resource) is not valid");
        return (*m_resource.Get());
    }

    template <typename t_info>
    void graphic_resource_t<t_info>::set_resource(Microsoft::WRL::ComPtr<ID3D12Resource> const& resource)
    {
        auto info = t_info{};
        auto state = D3D12_RESOURCE_STATE_COMMON;

        create_info_from_resource(*resource.Get(), info, state);

        m_resource = resource;
        m_info = info;
        m_states = graphic_resource_state_t{ state, d3d12_resource_desc_utils_t::get_subresources_count((*resource.Get()).GetDesc()) };
    }

    template <typename t_info>
    auto graphic_resource_t<t_info>::get_info() const -> t_info const&
    {
        return m_info;
    }

    template <typename t_info>
    void graphic_resource_t<t_info>::set_info(t_info const& info)
    {
        auto resource = Microsoft::WRL::ComPtr<ID3D12Resource>{};
        auto state = D3D12_RESOURCE_STATE_COMMON;

        create_resource_from_info(info, resource, state);

        m_resource = resource;
        m_info = info;
        m_states = graphic_resource_state_t{ state, d3d12_resource_desc_utils_t::get_subresources_count((*resource.Get()).GetDesc()) };
    }

    template <typename t_info>
    auto graphic_resource_t<t_info>::set_state(D3D12_RESOURCE_STATES const state, std::optional<size_t> const index /*= {}*/) -> std::vector<D3D12_RESOURCE_BARRIER>
    {
        auto barriers = std::vector<D3D12_RESOURCE_BARRIER>{};

        for (auto const& transition_barrier : m_states.set_state(state, index))
        {
            auto& barrier = barriers.emplace_back();
            barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
            barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
            barrier.Transition = transition_barrier;
            barrier.Transition.pResource = m_resource.Get();
        }

        if (auto const uav_info = dynamic_cast<i_support_unordered_access_t const*const>(&m_info); uav_info && (*uav_info).get_support_unordered_access())
        {
            auto& barrier = barriers.emplace_back();
            barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_UAV;
            barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
            barrier.UAV.pResource = m_resource.Get();
        }

        return barriers;
    }
} // namespace aiva
