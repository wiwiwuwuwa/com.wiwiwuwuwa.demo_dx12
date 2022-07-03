#include <pch.h>
#include <layer1/engine.h>

int __stdcall wWinMain(HINSTANCE, HINSTANCE, PWSTR, int)
{
	aiva::layer1::Engine{};
	return EXIT_SUCCESS;
}
