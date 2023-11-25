#include <pch.h>
#include <aiva/layer1_ext/ro_shader_vert.h>

#include <aiva/layer1/engine.h>
#include <aiva/layer1_ext/shader_consts.h>
#include <aiva/layer1_ext/shader_ia_input_layout_desc.h>
#include <aiva/layer1_ext/shader_meta_for_code.h>
#include <aiva/utils/asserts.h>
#include <aiva/utils/logger.h>
#include <aiva/utils_ext/object_utils.h>

namespace aiva::layer1_ext
{
	using namespace aiva::utils;
	using namespace aiva::utils_ext;

	RoShaderVert::RoShaderVert(aiva::layer1::Engine& engine) : mEngine{ engine }
	{

	}

	RoShaderVert::~RoShaderVert()
	{

	}

	void RoShaderVert::DeserealizeFromBinary(boost::span<const std::byte> const& resourceBin)
	{
		Asserts::CheckBool(!std::empty(resourceBin), "Resource bin is not valid");

		auto shaderBytecode = winrt::com_ptr<ID3DBlob>{};
		auto errorMessage = winrt::com_ptr<ID3DBlob>{};

		auto const result = D3DCompile
		(
			std::data(resourceBin),
			std::size(resourceBin),
			{},
			{},
			D3D_COMPILE_STANDARD_FILE_INCLUDE,
			ShaderConsts::EntryPointName,
			"vs_5_1",
			{},
			{},
			shaderBytecode.put(),
			errorMessage.put()
		);

		if (FAILED(result))
		{
			Asserts::CheckBool(errorMessage, "Error message is not valid");
			Logger::LogToDebugConsole(static_cast<const char*>(errorMessage->GetBufferPointer()));
			winrt::throw_hresult(result);
		}

		mBytecode = shaderBytecode;
		mMetadata = NewObject<ShaderMetaForCode>(reinterpret_cast<std::string::const_pointer>(std::data(resourceBin)), Engine());
		mInputLayout = NewObject<ShaderIAInputLayoutDesc>(shared_from_this(), Engine());

		Asserts::CheckBool(mBytecode, "Bytecode is not valid");
		Asserts::CheckBool(mMetadata, "Metadata is not valid");
		Asserts::CheckBool(mInputLayout, "Input layout is not valid");

		DirtyFlags().Dirty();
	}
}
