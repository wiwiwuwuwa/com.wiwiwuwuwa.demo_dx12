#pragma once
#include <pch.h>

#include <aiva/layer1/ro_scene_gltf_fwd.h>
#include <aiva/layer2/scene_actor_fwd.h>
#include <aiva/layer2/scene_system_fwd.h>
#include <aiva/utils/a_object.h>

namespace aiva::layer2
{
	struct World;
}

namespace aiva::layer2
{
	struct SceneSystem final : public aiva::utils::AObject
	{
	// ----------------------------------------------------
	// Main

	private:
		friend FactoryType;

	protected:
		SceneSystem(World const& world);

	public:
		~SceneSystem();

	private:
		World const& mWorld;

	// ----------------------------------------------------
	// Hierarchy

	public:
		SceneActorTypeShared CreateActor();

	private:
		std::vector<SceneActorTypeShared> mActors{};

	// ----------------------------------------------------
	// Scenes

	public:
		SceneActorTypeShared LoadScene(std::string const& path);

		SceneActorTypeShared LoadScene(aiva::layer1::RoSceneGltfTypeShared const& scene);

	private:
		std::vector<SceneActorTypeShared> LoadActors(aiva::layer1::RoSceneGltfTypeShared const& scene);

		void LoadActorsRelations(aiva::layer1::RoSceneGltfTypeShared const& scene, std::vector<SceneActorTypeShared> const& actors);

		void LoadActorsPositions(aiva::layer1::RoSceneGltfTypeShared const& scene, std::vector<SceneActorTypeShared> const& actors);

		void LoadActorsMeshRenderers(aiva::layer1::RoSceneGltfTypeShared const& scene, std::vector<SceneActorTypeShared> const& actors);

		void LoadActorsCameras(aiva::layer1::RoSceneGltfTypeShared const& scene, std::vector<SceneActorTypeShared> const& actors);

		void LoadActosComponents(aiva::layer1::RoSceneGltfTypeShared const& scene, std::vector<SceneActorTypeShared> const& actors);

		SceneActorTypeShared LoadActorsRoot(aiva::layer1::RoSceneGltfTypeShared const& scene, std::vector<SceneActorTypeShared> const& actors);
	};
}
