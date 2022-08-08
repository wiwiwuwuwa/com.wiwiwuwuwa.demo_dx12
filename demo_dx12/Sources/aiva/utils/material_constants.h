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
		static constexpr auto const* const AIVA_BUFFER_CONSTANTS_PER_OBJECT = "AIVA_B00_CONSTANTS_PER_OBJECT";

		static constexpr auto const* const AIVA_BUFFER_MESH_INDICES = "AIVA_T00_INDICES";

		static constexpr auto const* const AIVA_BUFFER_MESH_VERTICES = "AIVA_T01_VERTICES";

	// ----------------------------------------------------
	// Constants To Fields Inside Buffers

	public:
		static constexpr auto const* const AIVA_CONSTANT_MVP = "AIVA_M00_MVP";
	};
}