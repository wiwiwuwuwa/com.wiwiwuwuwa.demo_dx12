#pragma once
#include <pch.h>

#include <aiva/layer1/ro_json_fwd.h>
#include <aiva/layer2/e_scene_component_type.h>
#include <aiva/layer2/scene_actor_fwd.h>
#include <aiva/layer2/scene_component_fwd.h>

namespace aiva::layer2
{
	struct SceneActorUtils final
	{
	// ----------------------------------------------------
	// Main

	private:
		SceneActorUtils();

		~SceneActorUtils();

	private:
		static SceneActorUtils& Instance();

	// ----------------------------------------------------
	// Creating Components From Path

	public:
		static std::vector<SceneComponentTypeShared> CreateComponents(SceneActorTypeShared const& actor, std::string const& path);

	// ----------------------------------------------------
	// Creating Components From Json

	public:
		static std::vector<SceneComponentTypeShared> CreateComponents(SceneActorTypeShared const& actor, aiva::layer1::RoJsonTypeShared const& json);

		static std::vector<SceneComponentTypeShared> CreateComponents(SceneActorTypeShared const& actor, nlohmann::json const& json);

		static SceneComponentTypeShared CreateComponent(SceneActorTypeShared const& actor, nlohmann::json const& json);

	// ----------------------------------------------------
	// Creating Components From Enum

	public:
		static std::vector<SceneComponentTypeShared> CreateComponents(SceneActorTypeShared const& actor, std::vector<ESceneComponentType> const& componentTypes);

		static SceneComponentTypeShared CreateComponent(SceneActorTypeShared const& actor, ESceneComponentType const& componentType);

	private:
		using ComponentsFromEnumFactoryFunc = std::function<SceneComponentTypeShared(SceneActorTypeShared const&)>;

		using ComponentsFromEnumFactoryDict = std::unordered_map<ESceneComponentType, ComponentsFromEnumFactoryFunc>;

	private:
		void InitializeCreatingComponentsFromEnum();

		void TerminateCreatingComponentsFromEnum();

	private:
		template <typename TComponentType>
		void RegisterComponentsFromEnumFactoryFunc(ESceneComponentType const componentType);

	private:
		ComponentsFromEnumFactoryDict mComponentsFromEnumFactoryDict{};
	};
}

// --------------------------------------------------------

namespace aiva::layer2
{
	template <typename TComponentType>
	void SceneActorUtils::RegisterComponentsFromEnumFactoryFunc(ESceneComponentType const componentType)
	{
		mComponentsFromEnumFactoryDict.insert_or_assign(componentType, &SceneActorType::CreateComponent<TComponentType>);
	}
}
