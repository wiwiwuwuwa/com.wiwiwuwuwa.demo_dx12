#include <pch.h>
#include <aiva2/_init.hpp>

int __stdcall wWinMain(HINSTANCE, HINSTANCE, PWSTR, int)
{
	aiva2::init();
	return EXIT_SUCCESS;
}
