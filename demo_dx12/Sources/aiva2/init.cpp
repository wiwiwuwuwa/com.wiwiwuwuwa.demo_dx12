#include <pch.h>
#include <aiva2/init.hpp>

#include <aiva2/core/asserts.hpp>
#include <aiva2/core/engine.hpp>
#include <aiva2/core/object_utils.hpp>

namespace aiva2
{
	void init()
	{
		auto const engine = core::new_object<core::engine_t>();
		core::asserts_t::check_true(engine, "engine is not valid");

		engine->run();
	}
}
