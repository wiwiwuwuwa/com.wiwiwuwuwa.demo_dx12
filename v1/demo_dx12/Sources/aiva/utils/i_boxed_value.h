#pragma once
#include <pch.h>

#include <aiva/utils/e_cache_flags.h>
#include <aiva/utils/t_ev_action.h>

namespace aiva::utils
{
	enum class EBoxedType;
}

namespace aiva::utils
{
	struct IBoxedValue
	{
	// ----------------------------------------------------
	// Main

	public:
		virtual ~IBoxedValue() = default;

	// ----------------------------------------------------
	// Type

	public:
		virtual EBoxedType Type() const = 0;

	// ----------------------------------------------------
	// Binary

	public:
		virtual boost::span<std::byte> Binary() = 0;

	// ----------------------------------------------------
	// Changes

	public:
		using ChangeActionType = TEvAction<ECacheFlags>;

	public:
		virtual ChangeActionType& OnCacheDataChanged() = 0;
	};
}
