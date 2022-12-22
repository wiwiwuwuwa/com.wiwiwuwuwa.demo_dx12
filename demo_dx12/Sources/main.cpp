#include <pch.h>
#include <aiva2/init.h>

using namespace aiva2::engine;

int __stdcall wWinMain(HINSTANCE, HINSTANCE, PWSTR, int)
{
	aiva2::RunEngine();
	return EXIT_SUCCESS;
}
