#pragma once
#include <pch.h>

namespace aiva::layer1_ext
{
	struct DxSubResourceUtils final
	{
	// ----------------------------------------------------
	// Main

	private:
		DxSubResourceUtils() = delete;

	// ----------------------------------------------------
	// Generic Fields

	public:
		static std::size_t ToSubResource(std::size_t const mipSlice, std::size_t const arraySlice, std::size_t const planeSlice, std::size_t const mipLevels, std::size_t const arraySize);
	};
}
