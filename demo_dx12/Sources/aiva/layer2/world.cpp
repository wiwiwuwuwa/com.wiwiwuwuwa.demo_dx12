#include <pch.h>
#include <aiva/layer2/world.h>

#include <aiva/layer1/engine.h>
#include <aiva/layer1/gca_do_everything.h>
#include <aiva/layer1/graphic_executor.h>
#include <aiva/utils/asserts.h>

aiva::layer2::World::World()
{
	mEngine = std::make_unique<aiva::layer1::Engine>();
	aiva::utils::Asserts::CheckBool(mEngine);

	InitializeRender();
}

aiva::layer2::World::~World()
{
	TerminateRender();

	aiva::utils::Asserts::CheckBool(mEngine);
	mEngine = {};
}

void aiva::layer2::World::Run()
{
	aiva::utils::Asserts::CheckBool(mEngine);
	mEngine->Run();
}

void aiva::layer2::World::InitializeRender()
{
	aiva::utils::Asserts::CheckBool(mEngine);
	mEngine->OnPrepareForRender().connect(boost::bind(&aiva::layer2::World::TickRender, this));
}

void aiva::layer2::World::TickRender()
{
	aiva::utils::Asserts::CheckBool(mEngine);

	aiva::layer1::GcaDoEveryting command{};
	mEngine->GraphicExecutor().ExecuteCommand(command);
}

void aiva::layer2::World::TerminateRender()
{
	aiva::utils::Asserts::CheckBool(mEngine);
	mEngine->OnPrepareForRender().disconnect(boost::bind(&aiva::layer2::World::TickRender, this));
}
