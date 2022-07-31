#include <pch.h>
#include <aiva/layer2/scene_system.h>

#include <aiva/layer1/ro_scene_glb.h>
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

void aiva::layer2::SceneSystem::LoadScene(aiva::layer1::RoSceneGlb const& scene)
{
	auto const& gltfModel = scene.Model();

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
}
