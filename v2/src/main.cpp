#include <cstdlib>
#include <windows.h>
#include <aiva/init.hpp>

int __stdcall wWinMain(HINSTANCE, HINSTANCE, PWSTR, int)
{
    aiva::init();
    return EXIT_SUCCESS;
}
