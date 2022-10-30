#pragma once
#include <pch.h>

namespace aiva::utils_ext
{
	struct IObject
	{
	// ----------------------------------------------------
	// Main

	protected:
		IObject() = default;

	public:
		virtual ~IObject() = default;
	};
}
