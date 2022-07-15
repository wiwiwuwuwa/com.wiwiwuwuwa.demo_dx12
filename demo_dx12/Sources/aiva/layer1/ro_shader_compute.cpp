#include <pch.h>
#include <aiva/layer1/ro_shader_compute.h>

#include <aiva/layer1/engine.h>
#include <aiva/utils/asserts.h>

aiva::layer1::RoShaderCompute::RoShaderCompute()
{

}

aiva::layer1::RoShaderCompute::~RoShaderCompute()
{

}

void aiva::layer1::RoShaderCompute::Deserealize(aiva::layer1::Engine const& engine, std::vector<std::byte> const& binaryData)
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
		"cs_5_1",
		0,
		0,
		shaderBytecode.put(),
		errorMessage.put()
	);

	if (FAILED(result))
	{
		winrt::check_bool(errorMessage);
		engine.LogToDebugConsole(static_cast<const char*>(errorMessage->GetBufferPointer()));
		winrt::throw_hresult(result);
	}

	mCachedBytecode = shaderBytecode;
}

winrt::com_ptr<ID3DBlob> const& aiva::layer1::RoShaderCompute::CachedBytecode() const
{
	return mCachedBytecode;
}
