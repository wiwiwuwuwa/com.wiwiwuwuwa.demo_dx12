#include <pch.h>
#include <aiva/layer2/scene_system.h>

#include <aiva/layer1/e_descriptor_heap_type.h>
#include <aiva/layer1/e_buffer_memory_type.h>
#include <aiva/layer1/engine.h>
#include <aiva/layer1/gr_buffer.h>
#include <aiva/layer1/grv_srv_to_buffer.h>
#include <aiva/layer1/resource_system.h>
#include <aiva/layer1/resource_view_heap.h>
#include <aiva/layer1/ro_material_graphic.h>
#include <aiva/layer1/ro_scene_gltf.h>
#include <aiva/layer1/scene_gltf_utils.h>
#include <aiva/layer2/sc_camera.h>
#include <aiva/layer2/sc_mesh_renderer.h>
#include <aiva/layer2/scene_actor.h>
#include <aiva/layer2/scene_actor_utils.h>
#include <aiva/layer2/world.h>
#include <aiva/utils/asserts.h>

namespace aiva::layer2
{
	using namespace aiva::layer1;
	using namespace aiva::utils;

	SceneSystem::SceneSystem(World const& world) : AObject{}, mWorld{ world }
	{

	}

	SceneSystem::~SceneSystem()
	{

	}

	SceneActorTypeShared SceneSystem::CreateActor()
	{
		return mActors.emplace_back(new SceneActorType{ mWorld });
	}

	SceneActorTypeShared SceneSystem::LoadScene(std::string const& path)
	{
		Asserts::CheckBool(!std::empty(path), "Path is not valid");

		auto const resource = mWorld.Engine().ResourceSystem().GetResource<RoSceneGltfType>(path);
		Asserts::CheckBool(resource, "Resource is not valid");

		auto const scene = LoadScene(resource);
		Asserts::CheckBool(scene, "Scene is not valid");

		return scene;
	}

	SceneActorTypeShared SceneSystem::LoadScene(RoSceneGltfTypeShared const& scene)
	{
		Asserts::CheckBool(scene, "Scene is not valid");

		auto const actors = LoadActors(scene);
		LoadActorsRelations(scene, actors);
		LoadActorsPositions(scene, actors);
		LoadActorsMeshRenderers(scene, actors);
		LoadActorsCameras(scene, actors);
		return LoadActorsRoot(scene, actors);
	}

	std::vector<SceneActorTypeShared> SceneSystem::LoadActors(aiva::layer1::RoSceneGltfTypeShared const& scene)
	{
		auto actors = std::vector<SceneActorTypeShared>();

		for (std::size_t i = {}; i < std::size(scene->Model().nodes); i++)
		{
			auto const& glNode = scene->Model().nodes.at(i);
			auto const& glExtras = glNode.extras;

			auto actor = SceneActorTypeShared{};

			static constexpr char const* const PATH_KEY = "path";
			if (glExtras.Has(PATH_KEY))
			{
				auto const& glPath = glExtras.Get(PATH_KEY);
				auto const& path = glPath.Get<std::string>();

				actor = LoadScene(path);
			}
			else
			{
				actor = CreateActor();
			}

			Asserts::CheckBool(actor, "Actor is not valid");
			actors.emplace_back(actor);
		}

		return actors;
	}

	void SceneSystem::LoadActorsRelations(aiva::layer1::RoSceneGltfTypeShared const& scene, std::vector<SceneActorTypeShared> const& actors)
	{
		for (std::size_t i = {}; i < std::size(scene->Model().nodes); i++)
		{
			auto const& glParentNode = scene->Model().nodes.at(i);
			auto const& parentActor = actors.at(i);

			for (std::size_t j = {}; j < std::size(glParentNode.children); j++)
			{
				auto const& glChildNode = glParentNode.children[j];
				auto const& childActor = actors.at(glChildNode);

				childActor->Parent(parentActor);
			}
		}
	}

	void SceneSystem::LoadActorsPositions(aiva::layer1::RoSceneGltfTypeShared const& scene, std::vector<SceneActorTypeShared> const& actors)
	{
		for (std::size_t i = {}; i < std::size(scene->Model().nodes); i++)
		{
			auto const& glNode = scene->Model().nodes.at(i);
			auto const& actor = actors.at(i);

			actor->Name(glNode.name);

			if (glNode.translation.size() == glm::vec3::length())
			{
				auto const localPosition = glm::dvec3{ glNode.translation.at(0), glNode.translation.at(1), glNode.translation.at(2) };
				actor->LocalPosition(localPosition);
			}

			if (glNode.rotation.size() == glm::quat::length())
			{
				auto const localRotation = glm::dquat{ glNode.rotation.at(3), glNode.rotation.at(0), glNode.rotation.at(1), glNode.rotation.at(2) };
				actor->LocalRotation(localRotation);
			}

			if (glNode.scale.size() == glm::vec3::length())
			{
				auto const localScale = glm::dvec3{ glNode.scale.at(0), glNode.scale.at(1), glNode.scale.at(2) };
				actor->LocalScale(localScale);
			}
		}
	}

	void SceneSystem::LoadActorsMeshRenderers(aiva::layer1::RoSceneGltfTypeShared const& scene, std::vector<SceneActorTypeShared> const& actors)
	{
		auto const& models = SceneGltfUtils::LoadModels(scene);

		for (auto const& pair : models)
		{
			auto const& actor = actors.at(pair.first);
			auto const& meshRenderer = actor->CreateComponent<ScMeshRenderer>();
			meshRenderer->Material(pair.second);
		}
	}

	void SceneSystem::LoadActorsCameras(aiva::layer1::RoSceneGltfTypeShared const& scene, std::vector<SceneActorTypeShared> const& actors)
	{
		for (std::size_t i = {}; i < std::size(scene->Model().nodes); i++)
		{
			auto const& glNode = scene->Model().nodes.at(i);
			if (glNode.camera == -1)
			{
				continue;
			}

			auto const& glCamera = scene->Model().cameras.at(glNode.camera);
			if (glCamera.type != "perspective")
			{
				continue;
			}

			auto const& actor = actors.at(i);
			auto const& camera = actor->CreateComponent<ScCamera>();
			camera->FovY(glCamera.perspective.yfov);
			camera->ZNear(glCamera.perspective.znear);
			camera->ZFar(glCamera.perspective.zfar);
		}
	}

	void SceneSystem::LoadActosComponents(aiva::layer1::RoSceneGltfTypeShared const& scene, std::vector<SceneActorTypeShared> const& actors)
	{
		for (std::size_t i = {}; i < std::size(scene->Model().nodes); i++)
		{
			auto const& glNode = scene->Model().nodes.at(i);
			auto const& glExtras = glNode.extras;

			static constexpr char const* const COMPONENTS_PATH_KEY = "componentsPath";
			if (!glExtras.Has(COMPONENTS_PATH_KEY))
			{
				continue;
			}

			auto const& glComponentsPath = glExtras.Get(COMPONENTS_PATH_KEY);
			auto const& componentsPath = glComponentsPath.Get<std::string>();
			Asserts::CheckBool(!std::empty(componentsPath), "Components path is not valid");

			auto const& actor = actors.at(i);
			Asserts::CheckBool(actor, "Actor is not valid");

			SceneActorUtils::CreateComponents(actor, componentsPath);
		}
	}

	SceneActorTypeShared SceneSystem::LoadActorsRoot(aiva::layer1::RoSceneGltfTypeShared const& scene, std::vector<SceneActorTypeShared> const& actors)
	{
		auto const root = CreateActor();
		root->Name("Root");

		for (auto const& actor : actors)
		{
			if (!actor->Parent())
			{
				actor->Parent(root);
			}
		}

		return root;
	}
}
