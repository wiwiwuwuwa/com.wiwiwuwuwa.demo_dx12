#pragma once
#include <pch.h>

#include <aiva/utils/ev_action.h>

namespace aiva::utils
{
	struct IObjectChangeable
	{
	// ----------------------------------------------------
	// Main

	protected:
		IObjectChangeable();

	public:
		virtual ~IObjectChangeable();

	// ----------------------------------------------------
	// Change Event

	public:
		EvAction& OnChanged();

	private:
		EvAction mOnChanged{};
	};
}
