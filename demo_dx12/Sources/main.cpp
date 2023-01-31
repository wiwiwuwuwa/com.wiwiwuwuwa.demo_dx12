#include <pch.h>

#include <aiva2/init.h>

int __stdcall wWinMain(HINSTANCE, HINSTANCE, PWSTR, int)
{
	aiva2::init();
	return EXIT_SUCCESS;
}
