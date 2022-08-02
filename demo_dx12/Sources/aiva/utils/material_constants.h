#pragma once
#include <pch.h>

namespace aiva::utils
{
	struct MaterialConstants final
	{
	// ----------------------------------------------------
	// Main

	private:
		MaterialConstants() = delete;

	// ----------------------------------------------------
	// Constants To Buffers Inside Shaders

	public:
		static constexpr auto const* const AIVA_BUFFER_CONSTANT = "m00_AIVA_BUFFER_CONSTANT";

		static constexpr auto const* const AIVA_BUFFER_INDICES = "m01_AIVA_BUFFER_INDICES";

		static constexpr auto const* const AIVA_BUFFER_VERTICES = "m02_AIVA_BUFFER_VERTICES";

	// ----------------------------------------------------
	// Constants To Fields Inside Buffers

	public:
		static constexpr auto const* const AIVA_CONSTANT_MVP = "m00_AIVA_CONSTANT_MVP";
	};
}