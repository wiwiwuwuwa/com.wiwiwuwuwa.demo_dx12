#pragma once
#include <pch.h>

namespace aiva::layer1
{
	struct RoMaterialGraphic;

	using RoMaterialGraphicType = RoMaterialGraphic;
	using RoMaterialGraphicTypeShared = std::shared_ptr<RoMaterialGraphicType>;
	using RoMaterialGraphicTypeWeak = std::weak_ptr<RoMaterialGraphicType>;
}
