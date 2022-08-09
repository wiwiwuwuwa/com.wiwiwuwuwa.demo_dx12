#include <pch.h>
#include <aiva/utils/boxed_value_utils.h>

#include <aiva/layer1/shader_consts.h>
#include <aiva/utils/e_boxed_type.h>
#include <aiva/utils/i_boxed_value.h>
#include <aiva/utils/math_utils.h>

aiva::utils::BoxedValueUtils::BoxedValueUtils()
{
	InitializeTypeMapper();
}

aiva::utils::BoxedValueUtils::~BoxedValueUtils()
{
	TerminateTypeMapper();
}

aiva::utils::BoxedValueUtils& aiva::utils::BoxedValueUtils::Instance()
{
	static BoxedValueUtils instance{};
	return instance;
}

void aiva::utils::BoxedValueUtils::InitializeTypeMapper()
{
	mTypeOfDict = {};
	mCreateInstanceDict = {};

	RegisterTypeMapper<std::uint8_t>(EBoxedType::UINT8);
	RegisterTypeMapper<glm::u8vec2>(EBoxedType::UINT8X2);
	RegisterTypeMapper<glm::u8vec3>(EBoxedType::UINT8X3);
	RegisterTypeMapper<glm::u8vec4>(EBoxedType::UINT8X4);
	RegisterTypeMapper<std::int8_t>(EBoxedType::INT8);
	RegisterTypeMapper<glm::i8vec2>(EBoxedType::INT8X2);
	RegisterTypeMapper<glm::i8vec3>(EBoxedType::INT8X3);
	RegisterTypeMapper<glm::i8vec4>(EBoxedType::INT8X4);
	RegisterTypeMapper<std::uint16_t>(EBoxedType::UINT16);
	RegisterTypeMapper<glm::u16vec2>(EBoxedType::UINT16X2);
	RegisterTypeMapper<glm::u16vec3>(EBoxedType::UINT16X3);
	RegisterTypeMapper<glm::u16vec4>(EBoxedType::UINT16X4);
	RegisterTypeMapper<std::int16_t>(EBoxedType::INT16);
	RegisterTypeMapper<glm::i16vec2>(EBoxedType::INT16X2);
	RegisterTypeMapper<glm::i16vec3>(EBoxedType::INT16X3);
	RegisterTypeMapper<glm::i16vec4>(EBoxedType::INT16X4);
	RegisterTypeMapper<std::uint32_t>(EBoxedType::UINT32);
	RegisterTypeMapper<glm::u32vec2>(EBoxedType::UINT32X2);
	RegisterTypeMapper<glm::u32vec3>(EBoxedType::UINT32X3);
	RegisterTypeMapper<glm::u32vec4>(EBoxedType::UINT32X4);
	RegisterTypeMapper<std::int32_t>(EBoxedType::INT32);
	RegisterTypeMapper<glm::i32vec2>(EBoxedType::INT32X2);
	RegisterTypeMapper<glm::i32vec3>(EBoxedType::INT32X3);
	RegisterTypeMapper<glm::i32vec4>(EBoxedType::INT32X4);
	RegisterTypeMapper<std::uint64_t>(EBoxedType::UINT64);
	RegisterTypeMapper<glm::u64vec2>(EBoxedType::UINT64X2);
	RegisterTypeMapper<glm::u64vec3>(EBoxedType::UINT64X3);
	RegisterTypeMapper<glm::u64vec4>(EBoxedType::UINT64X4);
	RegisterTypeMapper<std::int64_t>(EBoxedType::INT64);
	RegisterTypeMapper<glm::i64vec2>(EBoxedType::INT64X2);
	RegisterTypeMapper<glm::i64vec3>(EBoxedType::INT64X3);
	RegisterTypeMapper<glm::i64vec4>(EBoxedType::INT64X4);
	RegisterTypeMapper<std::float_t>(EBoxedType::FLOAT32);
	RegisterTypeMapper<glm::f32vec2>(EBoxedType::FLOAT32X2);
	RegisterTypeMapper<glm::f32vec3>(EBoxedType::FLOAT32X3);
	RegisterTypeMapper<glm::f32vec4>(EBoxedType::FLOAT32X4);
	RegisterTypeMapper<std::double_t>(EBoxedType::FLOAT64);
	RegisterTypeMapper<glm::f64vec2>(EBoxedType::FLOAT64X2);
	RegisterTypeMapper<glm::f64vec3>(EBoxedType::FLOAT64X3);
	RegisterTypeMapper<glm::f64vec4>(EBoxedType::FLOAT64X4);
}

void aiva::utils::BoxedValueUtils::TerminateTypeMapper()
{
	mCreateInstanceDict = {};
	mTypeOfDict = {};
}

aiva::utils::EBoxedType aiva::utils::BoxedValueUtils::TypeOf(std::shared_ptr<IBoxedValue> const& boxedValue)
{
	Asserts::CheckBool(boxedValue, "Boxed value is not valid");
	return boxedValue->Type();
}

std::shared_ptr<aiva::utils::IBoxedValue> aiva::utils::BoxedValueUtils::CreateInstance(EBoxedType const boxedType)
{
	auto const iter = Instance().mCreateInstanceDict.find(boxedType);
	Asserts::CheckBool(iter != Instance().mCreateInstanceDict.end(), "Unknown CreateInstance type");

	return iter->second();
}

std::shared_ptr<aiva::utils::IBoxedValue> aiva::utils::BoxedValueUtils::CastTo(std::shared_ptr<IBoxedValue> const& boxedValue, EBoxedType const boxedType)
{
	Asserts::CheckBool(false, "TODO: Impl - BoxedValueUtils::CastTo");
	return {};
}

std::vector<std::byte> aiva::utils::BoxedValueUtils::SerializeToBinary(std::shared_ptr<IBoxedValue> const& boxedValue)
{
	Asserts::CheckBool(boxedValue, "Boxed value is not valid");

	auto const boxedBinary = boxedValue->Binary();
	Asserts::CheckBool(!std::empty(boxedBinary), "Boxed binary is empty");

	auto binaryData = std::vector<std::byte>{ std::size(boxedBinary) };
	Asserts::CheckBool(memcpy_s(std::data(binaryData), std::size(binaryData), std::data(boxedBinary), std::size(boxedBinary)) == 0, "Failed to memcpy_s");

	return binaryData;
}

void aiva::utils::BoxedValueUtils::DeserealizeFromBinary(std::shared_ptr<IBoxedValue> const& boxedValue, boost::span<const std::byte> const& binary)
{
	Asserts::CheckBool(boxedValue, "Boxed value is not valid");
	Asserts::CheckBool(!std::empty(binary), "Binary is empty");

	auto const boxedBinary = boxedValue->Binary();
	Asserts::CheckBool(!std::empty(boxedBinary), "Boxed binary is empty");

	Asserts::CheckBool(memcpy_s(std::data(boxedBinary), std::size(boxedBinary), std::data(binary), std::size(binary)) == 0, "Failed to memcpy_s");
}

std::shared_ptr<aiva::utils::IBoxedValue> aiva::utils::BoxedValueUtils::DeserealizeFromBinary(boost::span<const std::byte> const& binary, EBoxedType const boxedType)
{
	auto const boxedValue = CreateInstance(boxedType);
	Asserts::CheckBool(boxedValue, "Boxed value is not valid");

	DeserealizeFromBinary(boxedValue, binary);
	return boxedValue;
}
