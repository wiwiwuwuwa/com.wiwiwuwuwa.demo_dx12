#include <pch.h>
#include <aiva2/engine/asserts.h>
#include <aiva2/engine/i_window.h>
#include <aiva2/native/window_factory.h>

int __stdcall wWinMain(HINSTANCE, HINSTANCE, PWSTR, int)
{
	using namespace aiva2::engine;
	using namespace aiva2::native;

	auto const window = WindowFactory::Create();
	Asserts::IsTrue(window, "Window is not valid");
	window->Run();

	return EXIT_SUCCESS;
}
