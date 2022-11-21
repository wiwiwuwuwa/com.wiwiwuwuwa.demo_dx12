#include <pch.h>
#include <aiva/layer1_ext/dx_descriptor_range_type_utils.h>

#include <aiva/layer1_ext/e_shader_register_type_utils.h>
#include <aiva/utils/asserts.h>

namespace aiva::layer1_ext
{
	using namespace aiva::utils;

	DxDescriptorRangeTypeUtils::DxDescriptorRangeTypeUtils()
	{
		InitParseFromShaderRegisterType();
	}

	DxDescriptorRangeTypeUtils::~DxDescriptorRangeTypeUtils()
	{
		ShutParseFromShaderRegisterType();
	}

	DxDescriptorRangeTypeUtils& DxDescriptorRangeTypeUtils::Instance()
	{
		static DxDescriptorRangeTypeUtils instance{};
		return instance;
	}

	D3D12_DESCRIPTOR_RANGE_TYPE DxDescriptorRangeTypeUtils::Parse(EShaderRegisterType const shaderRegisterType)
	{
		Asserts::CheckBool(EShaderRegisterTypeUtils::IsValid(shaderRegisterType), "Shader register type is not valid");

		auto const iter = Instance().mShaderRegisterTypeToDescriptorRangeType.find(shaderRegisterType);
		Asserts::CheckBool(iter != std::cend(Instance().mShaderRegisterTypeToDescriptorRangeType), "Failed to parse descriptor range type from shader register type");

		return iter->second;
	}

	void DxDescriptorRangeTypeUtils::InitParseFromShaderRegisterType()
	{
		mShaderRegisterTypeToDescriptorRangeType = {};

		mShaderRegisterTypeToDescriptorRangeType.insert_or_assign(EShaderRegisterType::CBV, D3D12_DESCRIPTOR_RANGE_TYPE_CBV);
		mShaderRegisterTypeToDescriptorRangeType.insert_or_assign(EShaderRegisterType::SRV, D3D12_DESCRIPTOR_RANGE_TYPE_SRV);
		mShaderRegisterTypeToDescriptorRangeType.insert_or_assign(EShaderRegisterType::UAV, D3D12_DESCRIPTOR_RANGE_TYPE_UAV);
		mShaderRegisterTypeToDescriptorRangeType.insert_or_assign(EShaderRegisterType::Sampler, D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER);
	}

	void DxDescriptorRangeTypeUtils::ShutParseFromShaderRegisterType()
	{
		mShaderRegisterTypeToDescriptorRangeType = {};
	}
}
