#pragma once
#include <pch.h>

#include <aiva/utils/boxed_value_utils_hpp.h>

#include <aiva/utils/asserts.h>
#include <aiva/utils/e_boxed_type.h>
#include <aiva/utils/i_boxed_value.h>
#include <aiva/utils/object_utils.h>
#include <aiva/utils/t_boxed_value.h>

template <typename TValue>
void aiva::utils::BoxedValueUtils::RegisterTypeMapper(EBoxedType const boxedType)
{
	RegisterTypeOfElem<TValue>(boxedType);
	RegisterCreateInstanceElem<TValue>(boxedType);
}

template <typename TValue>
aiva::utils::EBoxedType aiva::utils::BoxedValueUtils::TypeOf()
{
	auto const iter = Instance().mTypeOfDict.find(typeid(TValue));
	Asserts::CheckBool(iter != Instance().mTypeOfDict.end(), "Unknown TypeOf type");

	return iter->second;
}

template <typename TValue>
void aiva::utils::BoxedValueUtils::RegisterTypeOfElem(EBoxedType const boxedType)
{
	mTypeOfDict.insert_or_assign(typeid(TValue), boxedType);
}

template <typename TValue>
std::shared_ptr<aiva::utils::TBoxedValue<TValue>> aiva::utils::BoxedValueUtils::CreateInstance()
{
	auto const basicInstance = CreateInstance(TypeOf<TValue>());
	Asserts::CheckBool(basicInstance, "Basic instance is not valid");

	auto const specificInstance = std::dynamic_pointer_cast<TBoxedValue<TValue>>(basicInstance);
	Asserts::CheckBool(specificInstance, "Specific instance is not valid");

	return specificInstance;
}

template <typename TValue>
void aiva::utils::BoxedValueUtils::RegisterCreateInstanceElem(EBoxedType const boxedType)
{
	mCreateInstanceDict.insert_or_assign(boxedType, &NewObject<TBoxedValue<TValue>>);
}

template <typename TDstValue>
std::shared_ptr<aiva::utils::TBoxedValue<TDstValue>> aiva::utils::BoxedValueUtils::CastTo(std::shared_ptr<IBoxedValue> const& boxedValue)
{
	if (!boxedValue)
	{
		return {};
	}

	auto const basicInstance = CastTo(boxedValue, TypeOf<TDstValue>());
	Asserts::CheckBool(basicInstance, "Basic instance is not valid");

	auto const specificInstance = std::dynamic_pointer_cast<TBoxedValue<TDstValue>>(basicInstance);
	Asserts::CheckBool(specificInstance, "Specific instance is not valid");

	return specificInstance;
}

template <typename TValue>
std::shared_ptr<aiva::utils::TBoxedValue<TValue>> aiva::utils::BoxedValueUtils::DeserealizeFromBinary(boost::span<const std::byte> const& binary)
{
	auto const basicInstance = DeserealizeFromBinary(binary, TypeOf<TValue>());
	Asserts::CheckBool(basicInstance, "Basic instance is not valid");

	auto const specificInstance = std::dynamic_pointer_cast<TBoxedValue<TValue>>(basicInstance);
	Asserts::CheckBool(specificInstance, "Specific instance is not valid");

	return specificInstance;
}
