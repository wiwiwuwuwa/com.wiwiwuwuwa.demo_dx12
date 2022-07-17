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
		static TEnum Or(TEnum const a, TEnum const b);
	};
}

// --------------------------------------------------------

template <typename TEnum>
TEnum aiva::utils::EnumUtils::Or(TEnum const a, TEnum const b)
{
	using NumberType = std::underlying_type_t<TEnum>;

	auto const resultNumber = static_cast<NumberType>(a) | static_cast<NumberType>(b);
	auto const resultEnum = static_cast<TEnum>(resultNumber);

	return resultEnum;
}
