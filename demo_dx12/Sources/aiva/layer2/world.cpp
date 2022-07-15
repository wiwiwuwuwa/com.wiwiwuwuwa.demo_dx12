#include <pch.h>
#include <aiva/layer2/world.h>

#include <aiva/layer1/engine.h>
#include <aiva/layer1/gca_dispatch.h>
#include <aiva/layer1/gca_do_everything.h>
#include <aiva/layer1/graphic_executor.h>
#include <aiva/layer1/resource_system.h>
#include <aiva/layer1/ro_material_compute.h>
#include <aiva/layer2/scene_system.h>
#include <aiva/utils/asserts.h>

aiva::layer2::World::World()
{
	
}

aiva::layer2::World::~World()
{
	
}

void aiva::layer2::World::Run()
{
	InitializeEngine();
	InitializeSystems();
	InitializeRender();

	aiva::utils::Asserts::CheckBool(mEngine);
	mEngine->Run();

	TerminateRender();
	TerminateSystems();
	TerminateEngine();
}

void aiva::layer2::World::InitializeEngine()
{
	mEngine = std::make_unique<aiva::layer1::Engine>();
	aiva::utils::Asserts::CheckBool(mEngine);
}

void aiva::layer2::World::TerminateEngine()
{
	aiva::utils::Asserts::CheckBool(mEngine);
	mEngine = {};
}

aiva::layer2::SceneSystem& aiva::layer2::World::SceneSystem() const
{
	aiva::utils::Asserts::CheckBool(mSceneSystem);
	return *mSceneSystem;
}

void aiva::layer2::World::InitializeSystems()
{
	mSceneSystem = aiva::layer2::SceneSystem::Create(weak_from_this());
	aiva::utils::Asserts::CheckBool(mSceneSystem);
}

void aiva::layer2::World::TerminateSystems()
{
	aiva::utils::Asserts::CheckBool(mSceneSystem);
	mSceneSystem = {};
}

void aiva::layer2::World::InitializeRender()
{
	aiva::utils::Asserts::CheckBool(mEngine);
	mEngine->OnPrepareForRender().connect(boost::bind(&aiva::layer2::World::TickRender, this));
}

void aiva::layer2::World::TickRender()
{
	aiva::utils::Asserts::CheckBool(mEngine);

	aiva::layer1::GcaDispatch gcaDispatch{};
	gcaDispatch.ThreadGroupCount = { 1, 1, 1 };
	gcaDispatch.ComputeMaterial = mEngine->ResourceSystem().GetResource<aiva::layer1::RoMaterialCompute>("resources/materials/checker.mat_cs");
	mEngine->GraphicExecutor().ExecuteCommand(gcaDispatch);

	aiva::layer1::GcaDoEveryting gcaDoEverything{};
	mEngine->GraphicExecutor().ExecuteCommand(gcaDoEverything);
}

void aiva::layer2::World::TerminateRender()
{
	aiva::utils::Asserts::CheckBool(mEngine);
	mEngine->OnPrepareForRender().disconnect(boost::bind(&aiva::layer2::World::TickRender, this));
}
