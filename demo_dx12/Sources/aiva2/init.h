#pragma once
#include <pch.h>

// --------------------------------------------------------

// Possible defines:
//    Plaftorms:
//        AIVA_PLATFORM_UWP

// --------------------------------------------------------

#include <aiva2/engine/asserts.h>
#include <aiva2/engine/object_utils.h>
#include <aiva2/native/engine.h>

namespace aiva2
{
	using namespace engine;
	using namespace native;

	void RunEngine()
	{
		auto const engine = NewObject<native::Engine>();
		Asserts::IsTrue(engine, "Engine is not valid");

		engine->Run();
	}
}
