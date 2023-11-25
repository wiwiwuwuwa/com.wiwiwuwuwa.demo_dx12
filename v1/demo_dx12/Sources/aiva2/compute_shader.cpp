#include <pch.h>
#include <aiva2/compute_shader.hpp>

#include <aiva2/assert.hpp>
#include <aiva2/d3d12_shader_bytecode_cs.hpp>
#include <aiva2/engine.hpp>
#include <aiva2/graphic_hardware.hpp>
#include <aiva2/logger.hpp>
#include <aiva2/shader_info.hpp>

namespace aiva2
{
	compute_shader_t::compute_shader_t(engine_t& engine, boost::span<std::byte const> const& binary)
		: impl_type{ engine, binary }
	{
		init_pipeline_state();
	}

	compute_shader_t::~compute_shader_t()
	{
		shut_pipeline_state();
	}

	auto compute_shader_t::get_pipeline_state_ref() const->ID3D12PipelineState&
	{
		assert_t::check_bool(m_pipeline_state, "m_pipeline_state is not valid");
		return (*m_pipeline_state);
	}

	auto compute_shader_t::get_pipeline_state_ptr() const->winrt::com_ptr<ID3D12PipelineState> const&
	{
		assert_t::check_bool(m_pipeline_state, "m_pipeline_state is not valid");
		return m_pipeline_state;
	}

	void compute_shader_t::init_pipeline_state()
	{
		auto const shader_bytecode_cs = d3d12_shader_bytecode_cs_t{ get_info() };

		auto pipeline_state_desc = D3D12_COMPUTE_PIPELINE_STATE_DESC{};
		pipeline_state_desc.pRootSignature = &get_root_signature_ref();
		pipeline_state_desc.CS = shader_bytecode_cs.get_data();
		pipeline_state_desc.NodeMask = {};
		pipeline_state_desc.CachedPSO = {};
		pipeline_state_desc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;

		auto const result = get_engine().get_graphic_hardware().get_device().CreateComputePipelineState
		(
			/*pDesc*/ &pipeline_state_desc,
			/*ppPipelineState*/ IID_PPV_ARGS(m_pipeline_state.put())
		);

		if (SUCCEEDED(result))
		{
			assert_t::check_bool(m_pipeline_state, "m_pipeline_state is not valid");
		}
		else
		{
			assert_t::check_hresult(result, "failed to create m_pipeline_state");
		}
	}

	void compute_shader_t::shut_pipeline_state()
	{
		assert_t::check_bool(m_pipeline_state, "m_pipeline_state is not valid");
		m_pipeline_state = {};
	}
}
