#include <pch.h>
#include <aiva/layer2/e_scene_component_type.h>

#include <aiva/utils/asserts.h>

namespace aiva::layer2
{
	using namespace aiva::utils;

	bool TryParse(std::string const& enumStr, ESceneComponentType& enumVal)
	{
		if (enumStr == "Camera")
		{
			enumVal = ESceneComponentType::Camera;
			return true;
		}

		if (enumStr == "MeshRenderer")
		{
			enumVal = ESceneComponentType::MeshRenderer;
			return true;
		}

		enumVal = {};
		return false;
	}

	std::string ToString(ESceneComponentType const enumVal)
	{
		switch (enumVal)
		{
		case ESceneComponentType::Camera:
			return "Camera";

		case ESceneComponentType::MeshRenderer:
			return "MeshRenderer";

		default:
			Asserts::CheckBool(false, "Enum value is not supported");
			return {};
		}
	}

	void from_json(nlohmann::json const& j, ESceneComponentType& p)
	{
		Asserts::CheckBool(TryParse(j.get<std::string>(), p), "Failed to parse enum from string");
	}

	void to_json(nlohmann::json& j, ESceneComponentType const& p)
	{
		j = ToString(p);
	}
}
