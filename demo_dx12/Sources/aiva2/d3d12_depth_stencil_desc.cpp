#include <pch.h>
#include <aiva2/d3d12_depth_stencil_desc.hpp>

#include <aiva2/shader_info.hpp>

namespace aiva2
{
    d3d12_depth_stencil_desc_t::d3d12_depth_stencil_desc_t(shader_info_t const&)
    {
        
    }

    d3d12_depth_stencil_desc_t::~d3d12_depth_stencil_desc_t()
    {
        
    }

    auto d3d12_depth_stencil_desc_t::get_data() const->D3D12_DEPTH_STENCIL_DESC
    {
        auto depth_stencil_desc = D3D12_DEPTH_STENCIL_DESC{};
        depth_stencil_desc.DepthEnable = TRUE;
        depth_stencil_desc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
        depth_stencil_desc.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
        depth_stencil_desc.StencilEnable = FALSE;
        depth_stencil_desc.StencilReadMask = D3D12_DEFAULT_STENCIL_READ_MASK;
        depth_stencil_desc.StencilWriteMask = D3D12_DEFAULT_STENCIL_WRITE_MASK;
        depth_stencil_desc.FrontFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
        depth_stencil_desc.FrontFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
        depth_stencil_desc.FrontFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
        depth_stencil_desc.FrontFace.StencilFunc = D3D12_COMPARISON_FUNC_ALWAYS;
        depth_stencil_desc.BackFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
        depth_stencil_desc.BackFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
        depth_stencil_desc.BackFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
        depth_stencil_desc.BackFace.StencilFunc = D3D12_COMPARISON_FUNC_ALWAYS;

        return depth_stencil_desc;
    }
}
