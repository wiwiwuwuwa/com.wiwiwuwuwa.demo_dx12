#pragma once
#include <pch.h>

#include <aiva/utils/ev_action.h>

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
		virtual EvAction& OnChanged() = 0;
	};
}
