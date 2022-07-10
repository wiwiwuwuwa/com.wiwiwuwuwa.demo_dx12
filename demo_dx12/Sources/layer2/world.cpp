#include <pch.h>
#include <layer2/world.h>

#include <layer1/engine.h>
#include <utils/asserts.h>

aiva::layer2::World::World()
{
	mEngine = std::make_unique<aiva::layer1::Engine>();
	aiva::utils::Asserts::CheckBool(mEngine);
}

aiva::layer2::World::~World()
{
	aiva::utils::Asserts::CheckBool(mEngine);
	mEngine = {};
}

void aiva::layer2::World::Run()
{
	aiva::utils::Asserts::CheckBool(mEngine);
	mEngine->Run();
}
