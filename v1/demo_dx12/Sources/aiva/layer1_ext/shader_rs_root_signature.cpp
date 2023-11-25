#include <pch.h>
#include <aiva/layer1_ext/shader_rs_root_signature.h>

#include <aiva/layer1/engine.h>
#include <aiva/layer1/graphic_hardware.h>
#include <aiva/layer1_ext/ro_material_gr.h>
#include <aiva/layer1_ext/shader_rs_root_signature_desc.h>
#include <aiva/utils/asserts.h>
#include <aiva/utils/logger.h>
#include <aiva/utils_ext/object_utils.h>

namespace aiva::layer1_ext
{
	using namespace aiva::utils;
	using namespace aiva::utils_ext;

	ShaderRSRootSignature::ShaderRSRootSignature(std::shared_ptr<RoMaterialGr> const& graphicMaterial, aiva::layer1::Engine& engine) : ShaderRSRootSignature{ NewObject<ShaderRSRootSignatureDesc>(graphicMaterial, engine), engine }
	{

	}

	ShaderRSRootSignature::ShaderRSRootSignature(std::shared_ptr<ShaderRSRootSignatureDesc> const& rootSignatureDesc, aiva::layer1::Engine& engine) : mEngine{ engine }
	{
		Asserts::CheckBool(rootSignatureDesc, "Root signature desc is not valid");

		auto const& device = engine.GraphicHardware().Device();
		Asserts::CheckBool(device, "Device is not valid");

		auto signatureBlob = winrt::com_ptr<ID3DBlob>{};
		auto errorMessages = winrt::com_ptr<ID3DBlob>{};

		auto const serializationResult = D3D12SerializeVersionedRootSignature(&rootSignatureDesc->NativeResource(), signatureBlob.put(), errorMessages.put());
		if (SUCCEEDED(serializationResult))
		{
			Asserts::CheckBool(signatureBlob, "Signature blob is not valid");
		}
		else
		{
			Asserts::CheckBool(errorMessages, "Error messages are not valid");
			Logger::LogToDebugConsole(static_cast<std::string::const_pointer>(errorMessages->GetBufferPointer()));
			winrt::throw_hresult(serializationResult);
		}

		winrt::check_hresult(device->CreateRootSignature({}, signatureBlob->GetBufferPointer(), signatureBlob->GetBufferSize(), IID_PPV_ARGS(&mNativeResource)));
		Asserts::CheckBool(mNativeResource, "Native resource is not valid");
	}

	ShaderRSRootSignature::~ShaderRSRootSignature()
	{

	}
}
