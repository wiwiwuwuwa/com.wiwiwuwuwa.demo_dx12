#include <pch.h>
#include <aiva2/compute_shader.hpp>

#include <aiva2/assert.hpp>
#include <aiva2/engine.hpp>
#include <aiva2/graphic_hardware.hpp>
#include <aiva2/logger.hpp>
#include <aiva2/shader_info.hpp>
#include <aiva2/shader_info_for_code.hpp>
#include <aiva2/shader_info_for_meta.hpp>
#include <aiva2/string_utils.hpp>

namespace aiva2
{
	compute_shader_t::compute_shader_t(engine_t& engine, boost::span<std::byte const> const& binary)
		: impl_type{ engine, binary }
	{
		init_bytecode_for_comp();
		init_pipeline_state();
	}

	compute_shader_t::~compute_shader_t()
	{
		shut_pipeline_state();
		shut_bytecode_for_comp();
	}

	auto compute_shader_t::get_bytecode_for_comp() const->ID3DBlob&
	{
		assert_t::check_bool(m_bytecode_for_comp, "m_bytecode_for_comp is not valid");
		return (*m_bytecode_for_comp);
	}

	void compute_shader_t::init_bytecode_for_comp()
	{
		auto error_message = winrt::com_ptr<ID3DBlob>{};

		auto const result = D3DCompile
		(
			/*pSrcData*/ std::data(get_info().get_code_block().get_text()),
			/*SrcDataSize*/ std::size(get_info().get_code_block().get_text()),
			/*pSourceName*/ {},
			/*pDefines*/ {},
			/*pInclude*/ {},
			/*pEntrypoint*/ get_info().get_meta_block().get_entry_for_comp().c_str(),
			/*pTarget*/ "cs_5_1",
			/*Flags1*/ {},
			/*Flags2*/ {},
			/*ppCode*/ m_bytecode_for_comp.put(),
			/*ppErrorMsgs*/ error_message.put()
		);

		if (SUCCEEDED(result))
		{
			assert_t::check_bool(m_bytecode_for_comp, "m_bytecode_for_comp is not valid");
		}
		else
		{
			assert_t::check_bool(error_message, "error_message is not valid");
			logger_t::log(string_utils_t::to_string(error_message));
			assert_t::check_hresult(result, "failed to compile m_bytecode_for_comp");
		}
	}

	void compute_shader_t::shut_bytecode_for_comp()
	{
		assert_t::check_bool(m_bytecode_for_comp, "Bytecode is not valid");
		m_bytecode_for_comp = {};
	}

	auto compute_shader_t::get_pipeline_state() const->ID3D12PipelineState&
	{
		assert_t::check_bool(m_pipeline_state, "m_pipeline_state is not valid");
		return (*m_pipeline_state);
	}

	void compute_shader_t::init_pipeline_state()
	{
		auto pipeline_state_desc = D3D12_COMPUTE_PIPELINE_STATE_DESC{};
		pipeline_state_desc.pRootSignature = &get_root_signature();
		pipeline_state_desc.CS.pShaderBytecode = get_bytecode_for_comp().GetBufferPointer();
		pipeline_state_desc.CS.BytecodeLength = get_bytecode_for_comp().GetBufferSize();
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
