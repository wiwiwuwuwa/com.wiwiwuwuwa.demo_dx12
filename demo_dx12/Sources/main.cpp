#include <pch.h>
#include <aiva/layer2/world.h>

int __stdcall wWinMain(HINSTANCE, HINSTANCE, PWSTR, int)
{
	// hello world

	auto const world = aiva::layer2::World::Create();
	world->Run();

	return EXIT_SUCCESS;
}
