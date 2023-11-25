#include <pch.h>
#include <aiva/utils_ext/e_value_type_utils.h>

#include <aiva/utils/asserts.h>

namespace aiva::utils_ext
{
	using namespace aiva::utils;

	EValueTypeUtils::EValueTypeUtils()
	{
		InitSizeOf();
		InitParseFromHLSL();
		InitParseFromGLTF();
	}

	EValueTypeUtils::~EValueTypeUtils()
	{
		ShutParseFromGLTF();
		ShutParseFromHLSL();
		ShutSizeOf();
	}

	EValueTypeUtils& EValueTypeUtils::Instance()
	{
		static EValueTypeUtils instance{};
		return instance;
	}

	bool EValueTypeUtils::IsValid(EValueType const value)
	{
		return value > EValueType::Unknown && value < EValueType::Maximum;
	}

	std::size_t EValueTypeUtils::SizeOf(EValueType const valueType)
	{
		Asserts::CheckBool(IsValid(valueType), "Value type is not valid");

		auto const iter = Instance().mValueTypeToSize.find(valueType);
		Asserts::CheckBool(iter != std::end(Instance().mValueTypeToSize), "Failed to find size of value type");

		return iter->second;
	}

	std::size_t EValueTypeUtils::SizeOf(aiva::layer1_ext::GlTypeDesc const& gltf)
	{
		auto const valueType = ParseFromGLTF(gltf);
		Asserts::CheckBool(IsValid(valueType), "Value type is not valid");

		return SizeOf(valueType);
	}

	void EValueTypeUtils::InitSizeOf()
	{
		mValueTypeToSize = {};

		mValueTypeToSize.insert_or_assign(EValueType::UInt8, sizeof(glm::u8vec1));
		mValueTypeToSize.insert_or_assign(EValueType::UInt16, sizeof(glm::u16vec1));
		mValueTypeToSize.insert_or_assign(EValueType::UInt32, sizeof(glm::u32vec1));
		mValueTypeToSize.insert_or_assign(EValueType::UInt32x2, sizeof(glm::u32vec2));
		mValueTypeToSize.insert_or_assign(EValueType::UInt32x3, sizeof(glm::u32vec3));
		mValueTypeToSize.insert_or_assign(EValueType::UInt32x4, sizeof(glm::u32vec4));
		mValueTypeToSize.insert_or_assign(EValueType::UInt32x4x4, sizeof(glm::u32mat4));
		mValueTypeToSize.insert_or_assign(EValueType::Float32, sizeof(glm::f32vec1));
		mValueTypeToSize.insert_or_assign(EValueType::Float32x2, sizeof(glm::f32vec2));
		mValueTypeToSize.insert_or_assign(EValueType::Float32x3, sizeof(glm::f32vec3));
		mValueTypeToSize.insert_or_assign(EValueType::Float32x4, sizeof(glm::f32vec4));
		mValueTypeToSize.insert_or_assign(EValueType::Float32x4x4, sizeof(glm::f32mat4));
	}

	void EValueTypeUtils::ShutSizeOf()
	{
		mValueTypeToSize = {};
	}

	EValueType EValueTypeUtils::ParseFromHLSL(std::string const& typeName)
	{
		Asserts::CheckBool(!std::empty(typeName), "Type name is not valid");

		auto const iter = Instance().mHLSLToValueType.find(typeName);
		Asserts::CheckBool(iter != std::end(Instance().mHLSLToValueType), "Failed to parse value type from hlsl");

		return iter->second;
	}

	void EValueTypeUtils::InitParseFromHLSL()
	{
		mHLSLToValueType = {};

		mHLSLToValueType.insert_or_assign("uint", EValueType::UInt32);
		mHLSLToValueType.insert_or_assign("uint2", EValueType::UInt32x2);
		mHLSLToValueType.insert_or_assign("uint3", EValueType::UInt32x3);
		mHLSLToValueType.insert_or_assign("uint4", EValueType::UInt32x4);
		mHLSLToValueType.insert_or_assign("uint4x4", EValueType::UInt32x4x4);
		mHLSLToValueType.insert_or_assign("float", EValueType::Float32);
		mHLSLToValueType.insert_or_assign("float2", EValueType::Float32x2);
		mHLSLToValueType.insert_or_assign("float3", EValueType::Float32x3);
		mHLSLToValueType.insert_or_assign("float4", EValueType::Float32x4);
		mHLSLToValueType.insert_or_assign("float4x4", EValueType::Float32x4x4);
	}

	void EValueTypeUtils::ShutParseFromHLSL()
	{
		mHLSLToValueType = {};
	}

	EValueType EValueTypeUtils::ParseFromGLTF(aiva::layer1_ext::GlTypeDesc const& gltf)
	{
		auto const iter = Instance().mGLTFToValueType.find(gltf);
		Asserts::CheckBool(iter != std::end(Instance().mGLTFToValueType), "Failed to parse value type from gltf");

		return iter->second;
	}

	void EValueTypeUtils::InitParseFromGLTF()
	{
		mGLTFToValueType = {};

		mGLTFToValueType.insert_or_assign({ TINYGLTF_TYPE_SCALAR, TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE }, EValueType::UInt8);
		mGLTFToValueType.insert_or_assign({ TINYGLTF_TYPE_SCALAR, TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT }, EValueType::UInt16);
		mGLTFToValueType.insert_or_assign({ TINYGLTF_TYPE_SCALAR, TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT }, EValueType::UInt32);
		mGLTFToValueType.insert_or_assign({ TINYGLTF_TYPE_VEC2, TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT }, EValueType::UInt32x2);
		mGLTFToValueType.insert_or_assign({ TINYGLTF_TYPE_VEC3, TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT }, EValueType::UInt32x3);
		mGLTFToValueType.insert_or_assign({ TINYGLTF_TYPE_VEC4, TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT }, EValueType::UInt32x4);
		mGLTFToValueType.insert_or_assign({ TINYGLTF_TYPE_MAT4, TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT }, EValueType::UInt32x4x4);
		mGLTFToValueType.insert_or_assign({ TINYGLTF_TYPE_SCALAR, TINYGLTF_COMPONENT_TYPE_FLOAT }, EValueType::Float32);
		mGLTFToValueType.insert_or_assign({ TINYGLTF_TYPE_VEC2, TINYGLTF_COMPONENT_TYPE_FLOAT }, EValueType::Float32x2);
		mGLTFToValueType.insert_or_assign({ TINYGLTF_TYPE_VEC3, TINYGLTF_COMPONENT_TYPE_FLOAT }, EValueType::Float32x3);
		mGLTFToValueType.insert_or_assign({ TINYGLTF_TYPE_VEC4, TINYGLTF_COMPONENT_TYPE_FLOAT }, EValueType::Float32x4);
		mGLTFToValueType.insert_or_assign({ TINYGLTF_TYPE_MAT4, TINYGLTF_COMPONENT_TYPE_FLOAT }, EValueType::Float32x4x4);
	}

	void EValueTypeUtils::ShutParseFromGLTF()
	{
		mGLTFToValueType = {};
	}
}
