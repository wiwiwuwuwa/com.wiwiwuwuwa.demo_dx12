#include <pch.h>
#include <aiva/layer2_ext/sc_scene.h>

#include <aiva/layer1/engine.h>
#include <aiva/layer1_ext/resource_system.h>
#include <aiva/layer1_ext/ro_scene_gltf.h>
#include <aiva/layer2/world.h>
#include <aiva/utils/asserts.h>

namespace aiva::layer2_ext
{
	using namespace aiva::layer1_ext;
	using namespace aiva::utils;

	ScScene::ScScene(aiva::layer2::World& world) : mWorld{ world }
	{

	}

	ScScene::~ScScene()
	{

	}

	void ScScene::DeserealizeFromJson(nlohmann::json const& resourceJsn)
	{
		//auto const gltf = mWorld.Engine().ResourceSystemExt().GetResource<RoSceneGltf>(resourceJsn.at("binary_path").get<std::string>());
		//Asserts::CheckBool(gltf, "GLTF scene is not valid");
	}
}
