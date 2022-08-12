#pragma once
#include <pch.h>

#include <aiva/layer1/ro_material_graphic_fwd.h>

namespace aiva::layer1
{
	struct RoMaterialGraphicUtils final
	{
	// ----------------------------------------------------
	// Main

	private:
		RoMaterialGraphicUtils() = delete;

	// ----------------------------------------------------
	// Utils

	public:
		static RoMaterialGraphicTypeShared Combine(RoMaterialGraphicTypeShared const& mainMaterial, RoMaterialGraphicTypeShared const& additionalMaterial);
	};
}
