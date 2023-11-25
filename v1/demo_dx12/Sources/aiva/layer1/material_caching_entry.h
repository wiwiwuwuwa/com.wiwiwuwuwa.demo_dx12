#pragma once
#include <pch.h>

#include <aiva/layer1/ro_material_graphic_fwd.h>

namespace aiva::layer1
{
	struct MaterialCachingEntry final
	{
	public:
		RoMaterialGraphicTypeShared Material{};

		std::uint64_t Tick{};

	public:
		MaterialCachingEntry();

		MaterialCachingEntry(RoMaterialGraphicTypeShared const& material, std::uint64_t const tick);
	};
}
