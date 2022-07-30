#include <pch.h>
#include <aiva/layer2/scene_system.h>

#include <aiva/layer2/scene_actor.h>
#include <aiva/layer2/world.h>

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
