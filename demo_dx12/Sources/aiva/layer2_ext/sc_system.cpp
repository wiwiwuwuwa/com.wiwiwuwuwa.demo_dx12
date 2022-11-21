#include <pch.h>
#include <aiva/layer2_ext/sc_system.h>

#include <aiva/layer1/engine.h>
#include <aiva/layer1_ext/resource_system.h>
#include <aiva/layer1_ext/ro_config_json.h>
#include <aiva/layer2/world.h>
#include <aiva/layer2_ext/sc_scene.h>
#include <aiva/utils/asserts.h>

namespace aiva::layer2_ext
{
	using namespace aiva::layer1_ext;
	using namespace aiva::utils;

	ScSystem::ScSystem(aiva::layer2::World& world) : mWorld{ world }
	{
		auto config = world.Engine().ResourceSystemExt().GetResource<RoConfigJson>("resources\\configs\\scene_system.json");
		Asserts::CheckBool(config, "Config is not valid");

		auto scene = world.Engine().ResourceSystemExt().GetResource<ScScene>(config->Data().at("scene_path").get<std::string>(), world);
		Asserts::CheckBool(scene, "Scene is not valid");

		mScene = scene;
	}

	ScSystem::~ScSystem()
	{

	}
}
