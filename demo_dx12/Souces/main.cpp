#include "pch.h"
#include "core/engine.h"

int __stdcall wWinMain(HINSTANCE, HINSTANCE, PWSTR, int)
{
	CoreApplication::Run(winrt::make<aiva::Engine>());
}
