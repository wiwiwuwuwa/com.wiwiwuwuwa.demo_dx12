#include <pch.h>
#include <aiva/layer2/scene_actor_utils.h>

#include <aiva/layer1/engine.h>
#include <aiva/layer1/resource_system.h>
#include <aiva/layer1/ro_json.h>
#include <aiva/layer2/e_scene_component_type.h>
#include <aiva/layer2/sc_camera.h>
#include <aiva/layer2/sc_mesh_renderer.h>
#include <aiva/layer2/scene_actor.h>
#include <aiva/layer2/world.h>
#include <aiva/utils/asserts.h>

namespace aiva::layer2
{
	using namespace aiva::layer1;
	using namespace aiva::utils;

	SceneActorUtils::SceneActorUtils()
	{
		InitializeCreatingComponentsFromEnum();
	}

	SceneActorUtils::~SceneActorUtils()
	{
		TerminateCreatingComponentsFromEnum();
	}

	SceneActorUtils& SceneActorUtils::Instance()
	{
		static SceneActorUtils instance{};
		return instance;
	}

	std::vector<SceneComponentTypeShared> SceneActorUtils::CreateComponents(SceneActorTypeShared const& actor, std::string const& path)
	{
		Asserts::CheckBool(actor, "Actor is not valid");
		Asserts::CheckBool(!std::empty(path), "Path is not valid");

		auto const json = actor->World().Engine().ResourceSystem().GetResource<RoJsonType>(path);
		Asserts::CheckBool(json, "Json resource is not valid");

		return CreateComponents(actor, json);
	}

	std::vector<SceneComponentTypeShared> SceneActorUtils::CreateComponents(SceneActorTypeShared const& actor, aiva::layer1::RoJsonTypeShared const& json)
	{
		Asserts::CheckBool(actor, "Actor is not valid");
		Asserts::CheckBool(json, "Json resource is not valid");

		return CreateComponents(actor, json->Json());
	}

	std::vector<SceneComponentTypeShared> SceneActorUtils::CreateComponents(SceneActorTypeShared const& actor, nlohmann::json const& json)
	{
		Asserts::CheckBool(actor, "Actor is not valid");

		auto components = std::vector<SceneComponentTypeShared>{};

		for (auto const& jsComponent : json)
		{
			auto const component = CreateComponent(actor, jsComponent);
			Asserts::CheckBool(component, "Component is not valid");

			components.emplace_back(component);
		}

		return components;
	}

	SceneComponentTypeShared SceneActorUtils::CreateComponent(SceneActorTypeShared const& actor, nlohmann::json const& json)
	{
		Asserts::CheckBool(actor, "Actor is not valid");

		auto const componentType = json.at("type").get<ESceneComponentType>();
		auto const component = CreateComponent(actor, componentType);
		Asserts::CheckBool(component, "Component is not valid");

		component->ParseFromJson(json);
		return component;
	}

	std::vector<SceneComponentTypeShared> SceneActorUtils::CreateComponents(SceneActorTypeShared const& actor, std::vector<ESceneComponentType> const& componentTypes)
	{
		Asserts::CheckBool(actor, "Actor is not valid");

		auto components = std::vector<SceneComponentTypeShared>{};

		for (auto const& componentType : componentTypes)
		{
			auto const component = CreateComponent(actor, componentType);
			Asserts::CheckBool(component, "Component is not valid");

			components.emplace_back(component);
		}

		return components;
	}

	SceneComponentTypeShared SceneActorUtils::CreateComponent(SceneActorTypeShared const& actor, ESceneComponentType const& componentType)
	{
		Asserts::CheckBool(actor, "Actor is not valid");

		auto const factory = Instance().mComponentsFromEnumFactoryDict.find(componentType);
		Asserts::CheckBool(factory != std::end(Instance().mComponentsFromEnumFactoryDict), "Factory is not valid");

		auto const component = factory->second(actor);
		Asserts::CheckBool(component, "Component is not valid");

		return component;
	}

	void SceneActorUtils::InitializeCreatingComponentsFromEnum()
	{
		mComponentsFromEnumFactoryDict = {};
		RegisterComponentsFromEnumFactoryFunc<ScCamera>(ESceneComponentType::Camera);
		RegisterComponentsFromEnumFactoryFunc<ScMeshRenderer>(ESceneComponentType::MeshRenderer);
	}

	void SceneActorUtils::TerminateCreatingComponentsFromEnum()
	{
		mComponentsFromEnumFactoryDict = {};
	}
}
