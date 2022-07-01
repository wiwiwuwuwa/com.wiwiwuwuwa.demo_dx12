#include <pch.h>
#include <layer0/app.h>

int __stdcall wWinMain(HINSTANCE, HINSTANCE, PWSTR, int)
{
	CoreApplication::Run(winrt::make<aiva::layer0::App>());
	return EXIT_SUCCESS;
}
