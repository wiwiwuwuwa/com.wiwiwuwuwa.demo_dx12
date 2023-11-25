#include <pch.h>
#include <aiva/utils/boxed_type_utils.h>

#include <aiva/utils/asserts.h>

namespace aiva::utils
{
	BoxedTypeUtils::BoxedTypeUtils()
	{
		InitializeGltfParsing();
	}

	BoxedTypeUtils::~BoxedTypeUtils()
	{
		TerminateGltfParsing();
	}

	BoxedTypeUtils& BoxedTypeUtils::Instance()
	{
		static BoxedTypeUtils instance{};
		return instance;
	}

	EBoxedType BoxedTypeUtils::Parse(GltfTypeDesc const& desc)
	{
		auto const& iter = Instance().mGltfParsingValues.find(desc);
		Asserts::CheckBool(iter != std::end(Instance().mGltfParsingValues), "Failed to parse gltf type desc");

		return iter->second;
	}

	void BoxedTypeUtils::InitializeGltfParsing()
	{
		mGltfParsingValues = {};

		mGltfParsingValues.insert_or_assign({ TINYGLTF_TYPE_SCALAR, TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE }, EBoxedType::UINT8);
		mGltfParsingValues.insert_or_assign({ TINYGLTF_TYPE_SCALAR, TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT }, EBoxedType::UINT16);
		mGltfParsingValues.insert_or_assign({ TINYGLTF_TYPE_SCALAR, TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT }, EBoxedType::UINT32);
		mGltfParsingValues.insert_or_assign({ TINYGLTF_TYPE_SCALAR, TINYGLTF_COMPONENT_TYPE_FLOAT }, EBoxedType::FLOAT32);
		mGltfParsingValues.insert_or_assign({ TINYGLTF_TYPE_VEC2, TINYGLTF_COMPONENT_TYPE_FLOAT }, EBoxedType::FLOAT32X2);
		mGltfParsingValues.insert_or_assign({ TINYGLTF_TYPE_VEC3, TINYGLTF_COMPONENT_TYPE_FLOAT }, EBoxedType::FLOAT32X3);
		mGltfParsingValues.insert_or_assign({ TINYGLTF_TYPE_VEC4, TINYGLTF_COMPONENT_TYPE_FLOAT }, EBoxedType::FLOAT32X4);
		mGltfParsingValues.insert_or_assign({ TINYGLTF_TYPE_MAT4, TINYGLTF_COMPONENT_TYPE_FLOAT }, EBoxedType::FLOAT32X4X4);
	}

	void BoxedTypeUtils::TerminateGltfParsing()
	{
		mGltfParsingValues = {};
	}
}
