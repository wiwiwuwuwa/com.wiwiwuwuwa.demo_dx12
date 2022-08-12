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

		static constexpr auto const* const AIVA_BUFFER_MESH_INDICES = "AIVA_T00_MESH_INDICES";

		static constexpr auto const* const AIVA_BUFFER_MESH_VERTICES = "AIVA_T01_MESH_VERTICES";

		static constexpr auto const* const AIVA_BUFFER_TEXTURE_MAIN = "AIVA_T02_TEXTURE_MAIN";

	// ----------------------------------------------------
	// Constants To Fields Inside Buffers

	public:
		static constexpr auto const* const AIVA_CONSTANT_M = "AIVA_M00_M";

		static constexpr auto const* const AIVA_CONSTANT_V = "AIVA_M01_V";

		static constexpr auto const* const AIVA_CONSTANT_P = "AIVA_M02_P";

		static constexpr auto const* const AIVA_CONSTANT_MVP = "AIVA_M03_MVP";
	};
}