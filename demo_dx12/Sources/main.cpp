#include <pch.h>
#include <aiva/layer2/world.h>

#include <aiva/layer1_ext/resource_system.h>
#include <aiva/layer1_ext/ro_texture_2d.h>
#include <aiva/utils_ext/object_utils.h>

int __stdcall wWinMain(HINSTANCE, HINSTANCE, PWSTR, int)
{
	auto const world = aiva::layer2::World::Create();

	auto const sys = aiva::utils_ext::NewObject<aiva::layer1_ext::ResourceSystem>(world->Engine());
	auto const res = sys->GetResource<aiva::layer1_ext::RoTexture2D>("resources/materials/avenger.mat_gs");

	world->Run();

	return EXIT_SUCCESS;
}
