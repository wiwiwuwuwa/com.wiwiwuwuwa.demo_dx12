#include <pch.h>
#include <aiva/layer1_ext/e_shader_register_type_utils.h>

#include <aiva/utils/asserts.h>

namespace aiva::layer1_ext
{
	using namespace aiva::utils;

	EShaderRegisterTypeUtils::EShaderRegisterTypeUtils()
	{
		InitParseFromHLSL();
	}

	EShaderRegisterTypeUtils::~EShaderRegisterTypeUtils()
	{
		ShutParseFromHLSL();
	}

	EShaderRegisterTypeUtils& EShaderRegisterTypeUtils::Instance()
	{
		static EShaderRegisterTypeUtils instance{};
		return instance;
	}

	bool EShaderRegisterTypeUtils::IsValid(EShaderRegisterType const value)
	{
		return value > EShaderRegisterType::Unknown && value < EShaderRegisterType::Max;
	}

	EShaderRegisterType EShaderRegisterTypeUtils::ParseFromHLSL(std::string const& typeName)
	{
		Asserts::CheckBool(!std::empty(typeName), "Type name is not valid");

		auto const iter = Instance().mHLSLToShaderRegisterType.find(typeName);
		Asserts::CheckBool(iter != std::cend(Instance().mHLSLToShaderRegisterType), "Failed to parse value type from hlsl");

		return iter->second;
	}

	void EShaderRegisterTypeUtils::InitParseFromHLSL()
	{
		mHLSLToShaderRegisterType = {};

		mHLSLToShaderRegisterType.insert_or_assign("b", EShaderRegisterType::CBV);
		mHLSLToShaderRegisterType.insert_or_assign("t", EShaderRegisterType::SRV);
		mHLSLToShaderRegisterType.insert_or_assign("u", EShaderRegisterType::UAV);
		mHLSLToShaderRegisterType.insert_or_assign("s", EShaderRegisterType::Sampler);
	}

	void EShaderRegisterTypeUtils::ShutParseFromHLSL()
	{
		mHLSLToShaderRegisterType = {};
	}
}
