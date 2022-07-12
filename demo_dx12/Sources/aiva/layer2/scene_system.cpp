#include <pch.h>
#include <aiva/layer2/scene_system.h>

#include <aiva/layer2/scene_actor.h>
#include <aiva/layer2/world.h>
#include <aiva/utils/asserts.h>

aiva::layer2::SceneSystem::SceneSystem(aiva::layer2::World const& world) : mWorld{ world }
{
	InitializeHierarchy();
}

aiva::layer2::SceneSystem::~SceneSystem()
{
	TerminateHierarchy();
}

std::shared_ptr<aiva::layer2::SceneActor> aiva::layer2::SceneSystem::SceneActor() const
{
	return mSceneActor;
}

void aiva::layer2::SceneSystem::InitializeHierarchy()
{
	mSceneActor = aiva::layer2::SceneActor::Create(mWorld, u8"🌍");
	aiva::utils::Asserts::CheckBool(mSceneActor);
}

void aiva::layer2::SceneSystem::TerminateHierarchy()
{
	aiva::utils::Asserts::CheckBool(mSceneActor);
	mSceneActor = {};
}
