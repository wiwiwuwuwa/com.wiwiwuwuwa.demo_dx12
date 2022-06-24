#include "pch.h"
#include "core/application.h"

int __stdcall wWinMain(HINSTANCE, HINSTANCE, PWSTR, int)
{
	CoreApplication::Run(winrt::make<aiva::Application>());
}
