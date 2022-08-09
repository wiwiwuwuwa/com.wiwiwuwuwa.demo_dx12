#pragma once
#include <pch.h>

#include <aiva/utils/t_boxed_value_hpp.h>

#include <aiva/utils/boxed_value_utils.h>
#include <aiva/utils/e_boxed_type.h>

template <typename TValue>
aiva::utils::TBoxedValue<TValue>::TBoxedValue(ValueType const& value /*= {}*/)
{
	Value(value);
}

template <typename TValue>
aiva::utils::TBoxedValue<TValue>::~TBoxedValue()
{

}

template <typename TValue>
aiva::utils::EBoxedType aiva::utils::TBoxedValue<TValue>::Type() const
{
	return BoxedValueUtils::TypeOf<TValue>();
}

template <typename TValue>
typename aiva::utils::TBoxedValue<TValue>::ValueType const& aiva::utils::TBoxedValue<TValue>::Value() const
{
	return mValue;
}

template <typename TValue>
aiva::utils::TBoxedValue<TValue>& aiva::utils::TBoxedValue<TValue>::Value(ValueType const& value)
{
	mValue = value;
	return *this;
}

template <typename TValue>
boost::span<std::byte> aiva::utils::TBoxedValue<TValue>::Binary() const
{
	return boost::as_writable_bytes(boost::span{ &mValue, std::size_t{ 1 } });
}
