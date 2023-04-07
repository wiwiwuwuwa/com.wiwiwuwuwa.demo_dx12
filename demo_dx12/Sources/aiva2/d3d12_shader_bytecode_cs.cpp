#include <pch.h>
#include <aiva2/d3d12_shader_bytecode_cs.hpp>

#include <aiva2/assert.hpp>
#include <aiva2/shader_info.hpp>
#include <aiva2/shader_info_for_code.hpp>
#include <aiva2/shader_info_for_meta.hpp>
#include <aiva2/string_utils.hpp>

namespace aiva2
{
    d3d12_shader_bytecode_cs_t::d3d12_shader_bytecode_cs_t(shader_info_t const& shader_info)
    {
        init_bytecode(shader_info);
    }

    d3d12_shader_bytecode_cs_t::~d3d12_shader_bytecode_cs_t()
    {
        shut_bytecode();
    }

    auto d3d12_shader_bytecode_cs_t::get_data() const->D3D12_SHADER_BYTECODE
    {
        assert_t::check_bool(m_bytecode, "(m_bytecode) is not valid");

        auto bytecode = D3D12_SHADER_BYTECODE{};
        bytecode.pShaderBytecode = (*m_bytecode).GetBufferPointer();
        bytecode.BytecodeLength = (*m_bytecode).GetBufferSize();
        return bytecode;
    }

    void d3d12_shader_bytecode_cs_t::init_bytecode(shader_info_t const& shader_info)
    {
        auto error_message = winrt::com_ptr<ID3DBlob>{};

		auto const result = D3DCompile
		(
			/*pSrcData*/ std::data(shader_info.get_code_block().get_text()),
			/*SrcDataSize*/ std::size(shader_info.get_code_block().get_text()),
			/*pSourceName*/ {},
			/*pDefines*/ {},
			/*pInclude*/ {},
			/*pEntrypoint*/ shader_info.get_meta_block().get_entry_for_comp().c_str(),
			/*pTarget*/ "cs_5_1",
			/*Flags1*/ {},
			/*Flags2*/ {},
			/*ppCode*/ m_bytecode.put(),
			/*ppErrorMsgs*/ error_message.put()
		);

		if (SUCCEEDED(result))
		{
			assert_t::check_bool(m_bytecode, "(m_bytecode) is not valid");
		}
		else
		{
			assert_t::check_bool(error_message, "(error_message) is not valid");
			logger_t::log(string_utils_t::to_string(error_message));
			assert_t::check_hresult(result, "failed to compile m_bytecode_for_comp");
		}
    }

    void d3d12_shader_bytecode_cs_t::shut_bytecode()
    {
        assert_t::check_bool(m_bytecode, "(m_bytecode) is not valid");
        m_bytecode = {};
    }
}
