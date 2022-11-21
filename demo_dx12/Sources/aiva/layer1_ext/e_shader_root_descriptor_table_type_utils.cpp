#include <pch.h>
#include <aiva/layer1_ext/e_shader_root_descriptor_table_type_utils.h>

#include <aiva/layer1_ext/e_shader_register_type_utils.h>
#include <aiva/utils/asserts.h>
#include <aiva/utils_ext/enum_utils.h>

namespace aiva::layer1_ext
{
	using namespace aiva::utils;
	using namespace aiva::utils_ext;

	EShaderRootDescriptorTableTypeUtils::EShaderRootDescriptorTableTypeUtils()
	{
		InitParseFromShaderRegisterType();
	}

	EShaderRootDescriptorTableTypeUtils::~EShaderRootDescriptorTableTypeUtils()
	{
		ShutParseFromShaderRegisterType();
	}

	EShaderRootDescriptorTableTypeUtils& EShaderRootDescriptorTableTypeUtils::Instance()
	{
		static EShaderRootDescriptorTableTypeUtils instance{};
		return instance;
	}

	bool EShaderRootDescriptorTableTypeUtils::IsValid(EShaderRootDescriptorTableType const value)
	{
		return value > EShaderRootDescriptorTableType::Unknown && value < EShaderRootDescriptorTableType::Max;
	}

	EShaderRootDescriptorTableType EShaderRootDescriptorTableTypeUtils::Parse(EShaderRegisterType const shaderRegisterType)
	{
		Asserts::CheckBool(EShaderRegisterTypeUtils::IsValid(shaderRegisterType), "Shader register type is not valid");

		auto const iter = Instance().mShaderRegisterTypeToShaderRootDescriptorTableType.find(shaderRegisterType);
		Asserts::CheckBool(iter != std::cend(Instance().mShaderRegisterTypeToShaderRootDescriptorTableType), "Failed to parse shader root descriptor table type from shader register type");

		return iter->second;
	}

	void EShaderRootDescriptorTableTypeUtils::InitParseFromShaderRegisterType()
	{
		mShaderRegisterTypeToShaderRootDescriptorTableType = {};

		mShaderRegisterTypeToShaderRootDescriptorTableType.insert_or_assign(EShaderRegisterType::CBV, EShaderRootDescriptorTableType::CbvSrvUav);
		mShaderRegisterTypeToShaderRootDescriptorTableType.insert_or_assign(EShaderRegisterType::SRV, EShaderRootDescriptorTableType::CbvSrvUav);
		mShaderRegisterTypeToShaderRootDescriptorTableType.insert_or_assign(EShaderRegisterType::UAV, EShaderRootDescriptorTableType::CbvSrvUav);
		mShaderRegisterTypeToShaderRootDescriptorTableType.insert_or_assign(EShaderRegisterType::Sampler, EShaderRootDescriptorTableType::Sampler);
	}

	void EShaderRootDescriptorTableTypeUtils::ShutParseFromShaderRegisterType()
	{
		mShaderRegisterTypeToShaderRootDescriptorTableType = {};
	}
}
