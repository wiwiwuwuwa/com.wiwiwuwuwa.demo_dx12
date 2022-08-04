#include <pch.h>
#include <aiva/layer2/world.h>

#include <aiva/layer1/e_descriptor_heap_type.h>
#include <aiva/layer1/engine.h>
#include <aiva/layer1/gca_dispatch.h>
#include <aiva/layer1/graphic_executor.h>
#include <aiva/layer1/graphic_hardware.h>
#include <aiva/layer1/graphic_pipeline.h>
#include <aiva/layer1/resource_system.h>
#include <aiva/layer1/resource_view_heap.h>
#include <aiva/layer1/resource_view_table.h>
#include <aiva/layer1/ro_material_compute.h>
#include <aiva/layer1/ro_scene_gltf.h>
#include <aiva/layer2/render_system.h>
#include <aiva/layer2/scene_system.h>
#include <aiva/utils/asserts.h>

aiva::layer2::World::World()
{
	InitializeEngine();
}

aiva::layer2::World::~World()
{
	TerminateEngine();
}

void aiva::layer2::World::Run()
{
	aiva::utils::Asserts::CheckBool(mEngine);
	mEngine->Run();
}

aiva::layer1::Engine& aiva::layer2::World::Engine() const
{
	aiva::utils::Asserts::CheckBool(mEngine);
	return *mEngine;
}

void aiva::layer2::World::InitializeEngine()
{
	mEngine = std::make_unique<aiva::layer1::Engine>();
	aiva::utils::Asserts::CheckBool(mEngine);

	mEngine->OnStart().connect(boost::bind(&aiva::layer2::World::OnEngineStart, this));
	mEngine->OnFinish().connect(boost::bind(&aiva::layer2::World::OnEngineFinish, this));
}

void aiva::layer2::World::TerminateEngine()
{
	aiva::utils::Asserts::CheckBool(mEngine);

	mEngine->OnFinish().disconnect(boost::bind(&aiva::layer2::World::OnEngineFinish, this));
	mEngine->OnStart().disconnect(boost::bind(&aiva::layer2::World::OnEngineStart, this));

	mEngine = {};
}

void aiva::layer2::World::OnEngineStart()
{
	InitializeSystems();
}

void aiva::layer2::World::OnEngineFinish()
{
	TerminateSystems();
}

aiva::layer2::SceneSystem& aiva::layer2::World::SceneSystem() const
{
	aiva::utils::Asserts::CheckBool(mSceneSystem);
	return *mSceneSystem;
}

aiva::layer2::RenderSystem& aiva::layer2::World::RenderSystem() const
{
	aiva::utils::Asserts::CheckBool(mSceneSystem);
	return *mRenderSystem;
}

void aiva::layer2::World::InitializeSystems()
{
	{
		mRenderSystem = RenderSystem::Create(*this);
		aiva::utils::Asserts::CheckBool(mRenderSystem);
	}

	{
		mSceneSystem = aiva::layer2::SceneSystem::Create(*this);
		aiva::utils::Asserts::CheckBool(mSceneSystem);

		auto const& sceneResource = mEngine->ResourceSystem().GetResource<aiva::layer1::RoSceneGltf>("resources\\scenes\\main.scene_gltf");
		aiva::utils::Asserts::CheckBool(sceneResource);

		mSceneSystem->LoadScene(*sceneResource);
	}
}

void aiva::layer2::World::TerminateSystems()
{
	{
		aiva::utils::Asserts::CheckBool(mSceneSystem);
		mSceneSystem = {};
	}

	{
		aiva::utils::Asserts::CheckBool(mRenderSystem);
		mRenderSystem = {};
	}
}
