#include <pch.h>
#include <aiva/layer2/world.h>

int __stdcall wWinMain(HINSTANCE, HINSTANCE, PWSTR, int)
{
	aiva::layer2::World world{};
	world.Run();

	return EXIT_SUCCESS;
}
