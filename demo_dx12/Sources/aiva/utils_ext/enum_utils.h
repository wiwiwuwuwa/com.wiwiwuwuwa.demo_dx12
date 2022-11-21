#pragma once
#include <pch.h>

namespace aiva::utils_ext
{
	struct EnumUtils final
	{
	// ----------------------------------------------------
	// Main

	private:
		EnumUtils() = delete;

	// ----------------------------------------------------
	// Validation

	public:
		template <typename TEnumType>
		static constexpr bool IsValid(TEnumType const val);

	// ----------------------------------------------------
	// Math

	public:
		template <typename TEnumType>
		static constexpr TEnumType AddByVal(TEnumType const src, std::underlying_type_t<TEnumType> const val);

		template <typename TEnumType>
		static constexpr void IncByRef(TEnumType& src);

		template <typename TEnumType>
		static constexpr TEnumType IncByVal(TEnumType const src);
	};
}

// --------------------------------------------------------

namespace aiva::utils_ext
{
	template <typename TEnumType>
	constexpr bool EnumUtils::IsValid(TEnumType const val)
	{
		return val > TEnumType::Unknown && val < TEnumType::Maximum;
	}

	template <typename TEnumType>
	constexpr TEnumType EnumUtils::AddByVal(TEnumType const src, std::underlying_type_t<TEnumType> const val)
	{
		using UnderlyingType = std::underlying_type_t<TEnumType>;
		return static_cast<TEnumType>(static_cast<UnderlyingType>(src) + val);
	}

	template <typename TEnumType>
	constexpr void EnumUtils::IncByRef(TEnumType& src)
	{
		src = AddByVal(src, 1);
	}

	template <typename TEnumType>
	constexpr TEnumType EnumUtils::IncByVal(TEnumType const src)
	{
		return AddByVal(src, 1);
	}
}
