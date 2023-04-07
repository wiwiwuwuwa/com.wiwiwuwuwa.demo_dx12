#include <pch.h>
#include <aiva2/d3d12_blend_desc.hpp>

#include <aiva2/shader_info.hpp>

namespace aiva2
{
    d3d12_blend_desc_t::d3d12_blend_desc_t(shader_info_t const&)
    {
        
    }

    d3d12_blend_desc_t::~d3d12_blend_desc_t()
    {
        
    }

    auto d3d12_blend_desc_t::get_data() const->D3D12_BLEND_DESC
    {
        auto blend_desc = D3D12_BLEND_DESC{};
        blend_desc.AlphaToCoverageEnable = FALSE;
        blend_desc.IndependentBlendEnable = FALSE;
        blend_desc.RenderTarget[0].BlendEnable = FALSE;
        blend_desc.RenderTarget[0].LogicOpEnable = FALSE;
        blend_desc.RenderTarget[0].SrcBlend = D3D12_BLEND_ONE;
        blend_desc.RenderTarget[0].DestBlend = D3D12_BLEND_ZERO;
        blend_desc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
        blend_desc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
        blend_desc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
        blend_desc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
        blend_desc.RenderTarget[0].LogicOp = D3D12_LOGIC_OP_NOOP;
        blend_desc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

        return blend_desc;
    }
}
