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
		static constexpr auto const* const MESH_ATTR_INDICES = "_MESH_ATTR_INDICES";

		static constexpr auto const* const MESH_ATTR_VERTICES = "_MESH_ATTR_VERTICES";
	};
}