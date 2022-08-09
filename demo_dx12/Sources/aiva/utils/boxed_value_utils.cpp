#include <pch.h>
#include <aiva/utils/boxed_value_utils.h>

#include <aiva/layer1/shader_consts.h>
#include <aiva/utils/e_boxed_type.h>
#include <aiva/utils/i_boxed_value.h>
#include <aiva/utils/math_utils.h>

aiva::utils::BoxedValueUtils::BoxedValueUtils()
{

}

aiva::utils::BoxedValueUtils::~BoxedValueUtils()
{

}

aiva::utils::BoxedValueUtils& aiva::utils::BoxedValueUtils::Instance()
{
	static BoxedValueUtils instance{};
	return instance;
}

aiva::utils::EBoxedType aiva::utils::BoxedValueUtils::TypeOf(std::shared_ptr<IBoxedValue> const& boxedValue)
{
	Asserts::CheckBool(boxedValue, "Boxed value is not valid");
	return boxedValue->Type();
}

std::shared_ptr<aiva::utils::IBoxedValue> aiva::utils::BoxedValueUtils::CreateInstance(EBoxedType const boxedType)
{
	Asserts::CheckBool(false, "TODO: Impl - BoxedValueUtils::CreateInstance");
	return {};
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
