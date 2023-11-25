#include <pch.h>
#include <aiva2/shader_base.hpp>

#include <aiva2/assert.hpp>
#include <aiva2/d3d12_root_signature_desc.hpp>
#include <aiva2/engine.hpp>
#include <aiva2/graphic_hardware.hpp>
#include <aiva2/shader_info.hpp>
#include <aiva2/shader_info_for_meta.hpp>
#include <aiva2/string_utils.hpp>

namespace aiva2
{
	shader_base_t::shader_base_t(engine_t& engine, boost::span<std::byte const> const& binary)
		: impl_type{ engine }
	{
		init_info(binary);
		init_root_signature();
	}

	shader_base_t::~shader_base_t()
	{
		shut_root_signature();
		shut_info();
	}

	auto shader_base_t::get_info() const->shader_info_t const&
	{
		assert_t::check_bool(m_info, "m_info is not valid");
		return *m_info;
	}

	void shader_base_t::init_info(boost::span<std::byte const> const& binary)
	{
		auto const text = string_utils_t::to_string(binary);
		assert_t::check_bool(!std::empty(text), "text is empty");
		
		m_info = std::make_shared<shader_info_t>(get_engine(), text);
		assert_t::check_bool(m_info, "m_info is not valid");
	}

	void shader_base_t::shut_info()
	{
		assert_t::check_bool(m_info, "m_info is not valid");
		m_info = {};
	}

	auto shader_base_t::get_root_signature_ref() const->ID3D12RootSignature&
	{
		assert_t::check_bool(m_root_signature, "m_root_signature is not valid");
		return (*m_root_signature);
	}

	auto shader_base_t::get_root_signature_ptr() const->winrt::com_ptr<ID3D12RootSignature> const&
	{
		assert_t::check_bool(m_root_signature, "m_root_signature is not valid");
		return m_root_signature;
	}

	void shader_base_t::init_root_signature()
	{
		auto const root_signature_desc = d3d12_root_signature_desc_t{ get_info() };

		auto root_signature_blob = winrt::com_ptr<ID3DBlob>{};
		auto error_message = winrt::com_ptr<ID3DBlob>{};
		
		auto const serialize_root_signature_result = D3D12SerializeVersionedRootSignature
		(
			/*pRootSignature*/ &root_signature_desc.get_root_signature_desc(),
			/*ppBlob*/ root_signature_blob.put(),
			/*ppErrorBlob*/ error_message.put()
		);
		
		if (SUCCEEDED(serialize_root_signature_result))
		{
			assert_t::check_bool(root_signature_blob, "root_signature_blob is not valid");
		}
		else
		{
			assert_t::check_bool(error_message, "error_message is not valid");
			logger_t::log(string_utils_t::to_string(error_message));
			assert_t::check_hresult(serialize_root_signature_result, "failed to serialize root_signature_blob");
		}

		auto const create_root_signature_result = get_engine().get_graphic_hardware().get_device().CreateRootSignature
		(
			/*NodeMask*/ {},
			/*pBlobWithRootSignature*/ (*root_signature_blob).GetBufferPointer(),
			/*blobLengthInBytes*/ (*root_signature_blob).GetBufferSize(),
			/*riid*/ IID_PPV_ARGS(&m_root_signature)
		);

		assert_t::check_hresult(create_root_signature_result, "failed to create m_root_signature");
		assert_t::check_bool(m_root_signature, "m_root_signature is not valid");
	}

	void shader_base_t::shut_root_signature()
	{
		assert_t::check_bool(m_root_signature, "m_root_signature is not valid");
		m_root_signature = {};
	}
}
