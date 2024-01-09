#pragma once
#include <d3d12.h>
#include <aiva/graphic_resource.hpp>
#include <aiva/graphic_resource_state.hpp>
#include <aiva/texture_2d_info.hpp>

namespace aiva
{
    class texture_2d_t : public implements_t<texture_2d_t, graphic_resource_t<texture_2d_info_t>>
    {
        // ------------------------------------------------

    protected:
        void create_resource_from_info(texture_2d_info_t const& in_info, Microsoft::WRL::ComPtr<ID3D12Resource>& out_resource, D3D12_RESOURCE_STATES& out_state) const override;

        void create_info_from_resource(ID3D12Resource& in_resource, texture_2d_info_t& out_info, D3D12_RESOURCE_STATES& out_state) const override;

        // ------------------------------------------------
    };
} // namespace aiva
