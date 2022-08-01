#include <pch.h>
#include <aiva/layer2/scene_system.h>

#include <aiva/layer1/e_gpu_descriptor_heap_type.h>
#include <aiva/layer1/e_gpu_resource_memory_type.h>
#include <aiva/layer1/engine.h>
#include <aiva/layer1/gr_buffer.h>
#include <aiva/layer1/grv_srv_to_buffer.h>
#include <aiva/layer1/resource_system.h>
#include <aiva/layer1/resource_view_heap.h>
#include <aiva/layer1/ro_material_graphic.h>
#include <aiva/layer1/ro_scene_gltf.h>
#include <aiva/layer1/scene_gltf_utils.h>
#include <aiva/layer1/shader_struct.h>
#include <aiva/layer2/sc_camera.h>
#include <aiva/layer2/sc_mesh_renderer.h>
#include <aiva/layer2/scene_actor.h>
#include <aiva/layer2/world.h>
#include <aiva/utils/asserts.h>

aiva::layer2::SceneSystem::SceneSystem(World const& world) : mWorld{ world }
{

}

aiva::layer2::SceneSystem::~SceneSystem()
{

}

aiva::layer2::SceneActor& aiva::layer2::SceneSystem::CreateActor()
{
	return *mActors.emplace_back(new SceneActor{ mWorld });
}

void aiva::layer2::SceneSystem::LoadScene(aiva::layer1::RoSceneGltf const& scene)
{
	auto const& gltfModel = scene.Model();

	// ----------------------------------------------------
	// Actors

	auto aivaNodes = std::vector<std::shared_ptr<SceneActor>>();

	for (std::size_t i = {}; i < gltfModel.nodes.size(); i++)
	{
		auto const& gltfNode = gltfModel.nodes.at(i);
		auto const& aivaNode = aivaNodes.emplace_back(CreateActor().shared_from_this());

		aivaNode->Name(gltfNode.name);

		if (gltfNode.translation.size() == glm::vec3::length())
		{
			auto const localPosition = glm::dvec3{ gltfNode.translation.at(0), gltfNode.translation.at(1), gltfNode.translation.at(2) };
			aivaNode->LocalPosition(localPosition);
		}

		if (gltfNode.rotation.size() == glm::quat::length())
		{
			auto const localRotation = glm::dquat{ gltfNode.rotation.at(3), gltfNode.rotation.at(0), gltfNode.rotation.at(1), gltfNode.rotation.at(2) };
			aivaNode->LocalRotation(localRotation);
		}

		if (gltfNode.scale.size() == glm::vec3::length())
		{
			auto const localScale = glm::dvec3{ gltfNode.scale.at(0), gltfNode.scale.at(1), gltfNode.scale.at(2) };
			aivaNode->LocalScale(localScale);
		}
	}
	for (std::size_t i = {}; i < gltfModel.nodes.size(); i++)
	{
		auto const& gltfParentNode = gltfModel.nodes.at(i);
		auto const& aivaParentNode = aivaNodes.at(i);

		for (std::size_t j = {}; j < gltfParentNode.children.size(); j++)
		{
			auto const& gltfChildIndex = gltfParentNode.children[j];
			auto const& aivaChildNode = aivaNodes.at(gltfChildIndex);

			aivaChildNode->Parent(aivaParentNode);
		}
	}

	// ----------------------------------------------------
	// Materials

	auto const& aivaMaterials = aiva::layer1::SceneGltfUtils::LoadMaterials(scene);

	for (auto const& pair : aivaMaterials)
	{
		auto& aivaNode = *aivaNodes.at(pair.first);
		auto& meshRenderer = aivaNode.CreateComponent<ScMeshRenderer>();
		meshRenderer.Material(pair.second);
	}

	// ----------------------------------------------------
	// Cameras

	for (std::size_t i = {}; i < std::size(gltfModel.nodes); i++)
	{
		auto const& gltfNode = gltfModel.nodes.at(i);
		if (gltfNode.camera == -1)
		{
			continue;
		}

		auto const& gltfCamera = gltfModel.cameras.at(gltfNode.camera);
		if (gltfCamera.type != "perspective")
		{
			continue;
		}

		auto& aivaNode = *aivaNodes.at(i);
		auto& aivaCamera = aivaNode.CreateComponent<ScCamera>();
		aivaCamera.FovY(gltfCamera.perspective.yfov);
		aivaCamera.ZNear(gltfCamera.perspective.znear);
		aivaCamera.ZFar(gltfCamera.perspective.zfar);
	}
}
