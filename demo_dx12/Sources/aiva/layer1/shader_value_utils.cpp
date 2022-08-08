#include <pch.h>
#include <aiva/layer1/shader_value_utils.h>

#include <aiva/layer1/e_value_type.h>
#include <aiva/layer1/engine.h>
#include <aiva/layer1/value_type_utils.h>
#include <aiva/layer1/t_shader_value.h>
#include <aiva/utils/asserts.h>
#include <aiva/utils/object_factory.h>

aiva::layer1::ShaderValueUtils::ShaderValueUtils()
{
	InitializeValueTypeFactories();
}

aiva::layer1::ShaderValueUtils::~ShaderValueUtils()
{
	TerminateValueTypeFactories();
}

aiva::layer1::ShaderValueUtils& aiva::layer1::ShaderValueUtils::Instance()
{
	static ShaderValueUtils instance{};
	return instance;
}

std::shared_ptr<aiva::layer1::IShaderValue> aiva::layer1::ShaderValueUtils::CreateFromGltf(int const glScalarCount, int const glScalarType)
{
	auto const valueType = ValueTypeUtils::ParseFromGltf(glScalarCount, glScalarType);
	return CreateFromValueType(valueType);
}

std::shared_ptr<aiva::layer1::IShaderValue> aiva::layer1::ShaderValueUtils::CreateFromValueType(EValueType const valueType)
{
	auto const& factoryIter = Instance().mValueTypeFactories.find(valueType);
	if (factoryIter == Instance().mValueTypeFactories.end())
	{
		aiva::utils::Asserts::CheckBool(false, "Unsupported value type");
		return {};
	}

	return factoryIter->second();
}

void aiva::layer1::ShaderValueUtils::InitializeValueTypeFactories()
{
	mValueTypeFactories = {};
	RegisterValueTypeFactory<std::uint8_t>(EValueType::UInt8);
	RegisterValueTypeFactory<std::uint16_t>(EValueType::UInt16);
	RegisterValueTypeFactory<std::uint32_t>(EValueType::UInt32);
	RegisterValueTypeFactory<float>(EValueType::Float32);
	RegisterValueTypeFactory<glm::vec2>(EValueType::Float32_x2);
	RegisterValueTypeFactory<glm::vec3>(EValueType::Float32_x3);
	RegisterValueTypeFactory<glm::vec4>(EValueType::Float32_x4);
	RegisterValueTypeFactory<glm::mat4>(EValueType::Float32_4x4);
}

void aiva::layer1::ShaderValueUtils::TerminateValueTypeFactories()
{
	mValueTypeFactories = {};
}
