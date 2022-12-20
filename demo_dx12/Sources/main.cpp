#include <pch.h>
#include <aiva2/engine/asserts.h>
#include <aiva2/engine/engine.h>
#include <aiva2/engine/object_utils.h>

using namespace aiva2::engine;

int __stdcall wWinMain(HINSTANCE, HINSTANCE, PWSTR, int)
{
	auto const engine = NewObject<Engine>();
	Asserts::IsTrue(engine, "Engine is not valid");

	engine->Run();

	return EXIT_SUCCESS;
}
