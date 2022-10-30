#pragma once
#include <pch.h>

namespace aiva::utils_ext
{
	struct ISerializableBinary
	{
	// ----------------------------------------------------
	// Main

	public:
		virtual void DeserealizeFromBinary(boost::span<const std::byte> const&) = NULL;
	};
}
