#pragma once
#include <pch.h>

#include <aiva/layer2/e_scene_component_type_fwd.h>

namespace aiva::layer2
{
	enum class ESceneComponentType
	{
		Camera,
		MeshRenderer,
	};

	// ----------------------------------------------------
	// String

	bool TryParse(std::string const& enumStr, ESceneComponentType& enumVal);

	std::string ToString(ESceneComponentType const enumVal);

	// ----------------------------------------------------
	// Json

	void from_json(nlohmann::json const& j, ESceneComponentType& p);

	void to_json(nlohmann::json& j, ESceneComponentType const& p);
}
