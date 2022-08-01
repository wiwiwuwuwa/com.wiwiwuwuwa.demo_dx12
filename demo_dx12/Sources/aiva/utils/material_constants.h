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
	// Constants

	public:
		static constexpr auto const* const AIVA_MESH_INDICES = "_AIVA_MESH_INDICES";

		static constexpr auto const* const AIVA_MESH_VERTICES = "_AIVA_MESH_VERTICES";
	};
}