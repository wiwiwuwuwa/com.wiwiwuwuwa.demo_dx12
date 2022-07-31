#include <pch.h>
#include <aiva/layer1/value_type_utils.h>

#include <aiva/layer1/e_value_type.h>
#include <aiva/utils/asserts.h>

const std::string aiva::layer1::ValueTypeUtils::KEY_SEP = "_";

aiva::layer1::ValueTypeUtils::ValueTypeUtils()
{
	InitializeGltfFactories();
}

aiva::layer1::ValueTypeUtils::~ValueTypeUtils()
{
	TerminateGltfFactories();
}

aiva::layer1::ValueTypeUtils& aiva::layer1::ValueTypeUtils::Instance()
{
	static ValueTypeUtils instance{};
	return instance;
}

aiva::layer1::EValueType aiva::layer1::ValueTypeUtils::ParseFromGltf(int const glScalarCount, int const glScalarType)
{
	auto const& factoryIter = Instance().mGltfFactories.find(CreateKeyFromGltf(glScalarCount, glScalarType));
	if (factoryIter == Instance().mGltfFactories.end())
	{
		aiva::utils::Asserts::CheckBool(false, "Unsupported gltf value type");
		return {};
	}

	return factoryIter->second;
}

std::string aiva::layer1::ValueTypeUtils::CreateKeyFromGltf(int const glScalarCount, int const glScalarType)
{
	return std::to_string(glScalarCount) + "_" + std::to_string(glScalarType);
}

void aiva::layer1::ValueTypeUtils::InitializeGltfFactories()
{
	mGltfFactories = {};
	mGltfFactories.insert_or_assign(CreateKeyFromGltf(TINYGLTF_TYPE_SCALAR, TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE), EValueType::UInt8);
	mGltfFactories.insert_or_assign(CreateKeyFromGltf(TINYGLTF_TYPE_SCALAR, TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT), EValueType::UInt16);
	mGltfFactories.insert_or_assign(CreateKeyFromGltf(TINYGLTF_TYPE_SCALAR, TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT), EValueType::UInt32);
	mGltfFactories.insert_or_assign(CreateKeyFromGltf(TINYGLTF_TYPE_SCALAR, TINYGLTF_COMPONENT_TYPE_FLOAT), EValueType::Float32);
	mGltfFactories.insert_or_assign(CreateKeyFromGltf(TINYGLTF_TYPE_VEC2, TINYGLTF_COMPONENT_TYPE_FLOAT), EValueType::Float32_x2);
	mGltfFactories.insert_or_assign(CreateKeyFromGltf(TINYGLTF_TYPE_VEC3, TINYGLTF_COMPONENT_TYPE_FLOAT), EValueType::Float32_x3);
	mGltfFactories.insert_or_assign(CreateKeyFromGltf(TINYGLTF_TYPE_VEC4, TINYGLTF_COMPONENT_TYPE_FLOAT), EValueType::Float32_x4);
	mGltfFactories.insert_or_assign(CreateKeyFromGltf(TINYGLTF_TYPE_MAT4, TINYGLTF_COMPONENT_TYPE_FLOAT), EValueType::Float32_4x4);
}

void aiva::layer1::ValueTypeUtils::TerminateGltfFactories()
{
	mGltfFactories = {};
}
