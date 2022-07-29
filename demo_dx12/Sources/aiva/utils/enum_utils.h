#pragma once
#include <pch.h>

namespace aiva::utils
{
	struct EnumUtils final
	{
	private:
		EnumUtils() = delete;

	public:
		template <typename TEnum>
		static TEnum And(TEnum const a, TEnum const b);

		template <typename TEnum>
		static TEnum Not(TEnum const value);

		template <typename TEnum>
		static TEnum Or(TEnum const a, TEnum const b);

		template <typename TEnum>
		static TEnum Xor(TEnum const a, TEnum const b);

		template <typename TEnum>
		static TEnum Clear(TEnum const value, TEnum const flag);

		template <typename TEnum>
		static bool Has(TEnum const value, TEnum const flag);

		template <typename TEnum>
		static TEnum New(TEnum const value, TEnum const flag);
	};
}

// --------------------------------------------------------

template <typename TEnum>
TEnum aiva::utils::EnumUtils::And(TEnum const a, TEnum const b)
{
	using NumberType = std::underlying_type_t<TEnum>;

	auto const resultNumber = static_cast<NumberType>(a) & static_cast<NumberType>(b);
	auto const resultEnum = static_cast<TEnum>(resultNumber);

	return resultEnum;
}

template <typename TEnum>
static TEnum aiva::utils::EnumUtils::Not(TEnum const value)
{
	using NumberType = std::underlying_type_t<TEnum>;

	auto const resultNumber = (~static_cast<NumberType>(value));
	auto const resultEnum = static_cast<TEnum>(resultNumber);

	return resultEnum;
}

template <typename TEnum>
TEnum aiva::utils::EnumUtils::Or(TEnum const a, TEnum const b)
{
	using NumberType = std::underlying_type_t<TEnum>;

	auto const resultNumber = static_cast<NumberType>(a) | static_cast<NumberType>(b);
	auto const resultEnum = static_cast<TEnum>(resultNumber);

	return resultEnum;
}

template <typename TEnum>
static TEnum  aiva::utils::EnumUtils::Xor(TEnum const a, TEnum const b)
{
	using NumberType = std::underlying_type_t<TEnum>;

	auto const resultNumber = static_cast<NumberType>(a) ^ static_cast<NumberType>(b);
	auto const resultEnum = static_cast<TEnum>(resultNumber);

	return resultEnum;
}

template <typename TEnum>
static TEnum aiva::utils::EnumUtils::Clear(TEnum const value, TEnum const flag)
{
	return And(value, Not(flag));
}

template <typename TEnum>
bool aiva::utils::EnumUtils::Has(TEnum const value, TEnum const flag)
{
	return And(value, flag) == flag;
}

template <typename TEnum>
static TEnum aiva::utils::EnumUtils::New(TEnum const value, TEnum const flag)
{
	return And(Not(value), flag);
}
