#include <pch.h>
#include <aiva/layer1/ro_shader_vertex.h>

#include <aiva/layer1/engine.h>
#include <aiva/utils/asserts.h>
#include <aiva/utils/logger.h>

aiva::layer1::RoShaderVertex::RoShaderVertex(aiva::layer1::Engine const& engine) : mEngine{ engine }
{

}

aiva::layer1::RoShaderVertex::~RoShaderVertex()
{

}

void aiva::layer1::RoShaderVertex::DeserealizeFromBinary(std::vector<std::byte> const& binaryData)
{
	aiva::utils::Asserts::CheckBool(!binaryData.empty());

	winrt::com_ptr<ID3DBlob> shaderBytecode{};
	winrt::com_ptr<ID3DBlob> errorMessage{};

	HRESULT const result = D3DCompile
	(
		binaryData.data(),
		binaryData.size(),
		nullptr,
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"main",
		"vs_5_1",
		0,
		0,
		shaderBytecode.put(),
		errorMessage.put()
	);

	if (FAILED(result))
	{
		winrt::check_bool(errorMessage);
		aiva::utils::Logger::LogToDebugConsole(static_cast<const char*>(errorMessage->GetBufferPointer()));
		winrt::throw_hresult(result);
	}

	mBytecode = shaderBytecode;
}

winrt::com_ptr<ID3DBlob> const& aiva::layer1::RoShaderVertex::Bytecode() const
{
	winrt::check_bool(mBytecode);
	return mBytecode;
}
