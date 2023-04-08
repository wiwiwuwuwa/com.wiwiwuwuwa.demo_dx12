#include <pch.h>
#include <aiva2/d3d12_rasterizer_desc.hpp>

#include <aiva2/shader_info.hpp>

namespace aiva2
{
    d3d12_rasterizer_desc_t::d3d12_rasterizer_desc_t(shader_info_t const&)
    {
        
    }

    d3d12_rasterizer_desc_t::~d3d12_rasterizer_desc_t()
    {
        
    }

    auto d3d12_rasterizer_desc_t::get_data() const->D3D12_RASTERIZER_DESC
    {
        auto rasterizer_desc = D3D12_RASTERIZER_DESC{};
        rasterizer_desc.FillMode = D3D12_FILL_MODE_SOLID;
        rasterizer_desc.CullMode = D3D12_CULL_MODE_BACK;
        rasterizer_desc.FrontCounterClockwise = TRUE;
        rasterizer_desc.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
        rasterizer_desc.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
        rasterizer_desc.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
        rasterizer_desc.DepthClipEnable = TRUE;
        rasterizer_desc.MultisampleEnable = FALSE;
        rasterizer_desc.AntialiasedLineEnable = FALSE;
        rasterizer_desc.ForcedSampleCount = 0;
        rasterizer_desc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

        return rasterizer_desc;
    }
}
