#include <pch.h>
#include <aiva2/graphic_shader.hpp>

#include <aiva2/assert.hpp>
#include <aiva2/d3d12_blend_desc.hpp>
#include <aiva2/d3d12_depth_stencil_desc.hpp>
#include <aiva2/d3d12_rasterizer_desc.hpp>
#include <aiva2/d3d12_shader_bytecode_ps.hpp>
#include <aiva2/d3d12_shader_bytecode_vs.hpp>
#include <aiva2/engine.hpp>
#include <aiva2/graphic_hardware.hpp>
#include <aiva2/logger.hpp>
#include <aiva2/shader_info.hpp>

namespace aiva2
{
    graphic_shader_t::graphic_shader_t(engine_t& engine, boost::span<std::byte const> const& binary)
        : impl_type{ engine, binary }
    {
        init_pipeline_state();
    }

    graphic_shader_t::~graphic_shader_t()
    {
        shut_pipeline_state();
    }

    auto graphic_shader_t::get_pipeline_state_ref() const->ID3D12PipelineState&
    {
        assert_t::check_bool(m_pipeline_state, "(m_pipeline_state) is not valid");
        return (*m_pipeline_state);
    }

    auto graphic_shader_t::get_pipeline_state_ptr() const->winrt::com_ptr<ID3D12PipelineState> const&
    {
        return m_pipeline_state;
    }

    void graphic_shader_t::init_pipeline_state()
    {
        auto const shader_bytecode_ps = d3d12_shader_bytecode_ps_t{ get_info() };
        auto const shader_bytecode_vs = d3d12_shader_bytecode_vs_t{ get_info() };
        auto const blend_desc = d3d12_blend_desc_t{ get_info() };
        auto const rasterizer_desc = d3d12_rasterizer_desc_t{ get_info() };
        auto const depth_stencil_desc = d3d12_depth_stencil_desc_t{ get_info() };

        auto pipeline_state_desc = D3D12_GRAPHICS_PIPELINE_STATE_DESC{};
        pipeline_state_desc.pRootSignature = &get_root_signature_ref();
        pipeline_state_desc.VS = shader_bytecode_vs.get_data();
        pipeline_state_desc.PS = shader_bytecode_ps.get_data();
        pipeline_state_desc.DS = {};
        pipeline_state_desc.HS = {};
        pipeline_state_desc.GS = {};
        pipeline_state_desc.StreamOutput = {};
        pipeline_state_desc.BlendState = blend_desc.get_data();
        pipeline_state_desc.SampleMask = UINT_MAX;
        pipeline_state_desc.RasterizerState = rasterizer_desc.get_data();
        pipeline_state_desc.DepthStencilState = depth_stencil_desc.get_data();
        // pipeline_state_desc.InputLayout = {};
        pipeline_state_desc.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;
        pipeline_state_desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
        // pipeline_state_desc.NumRenderTargets = {};
        // pipeline_state_desc.RTVFormats = {};
        // pipeline_state_desc.DSVFormat = {};
        pipeline_state_desc.SampleDesc = { 1, 0 };
        pipeline_state_desc.NodeMask = {};
        pipeline_state_desc.CachedPSO = {};
        pipeline_state_desc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;

        auto const result = get_engine().get_graphic_hardware().get_device().CreateGraphicsPipelineState
        (
            /*pDesc*/ &pipeline_state_desc,
            /*ppPipelineState*/ IID_PPV_ARGS(m_pipeline_state.put())
        );

        if (SUCCEEDED(result))
        {
            assert_t::check_bool(m_pipeline_state, "(m_pipeline_state) is not valid");
        }
        else
        {
            assert_t::check_hresult(result, "failed to create (m_pipeline_state)");
        }
    }

    void graphic_shader_t::shut_pipeline_state()
    {
        assert_t::check_bool(m_pipeline_state, "(m_pipeline_state) is not valid");
        m_pipeline_state = {};
    }
}
