#include <pch.h>
#include <aiva2/gpu_cmd_present.hpp>

#include <aiva2/assert.hpp>
#include <aiva2/engine.hpp>
#include <aiva2/graphic_hardware.hpp>
#include <aiva2/graphic_renderer.hpp>
#include <aiva2/render_target.hpp>

namespace aiva2
{
	gpu_cmd_present_t::gpu_cmd_present_t(engine_t& engine)
		: impl_type{ engine }
	{

	}

	gpu_cmd_present_t::~gpu_cmd_present_t()
	{

	}

	void gpu_cmd_present_t::execute()
	{
		execute_resource_barrier();
	}

	void gpu_cmd_present_t::execute_resource_barrier()
	{
		auto const& screen_target = get_engine().get_graphic_renderer().get_screen_target();
		assert_t::check_bool(screen_target, "screen_target is not valid");
		
		(*screen_target).shut_state_for_transition();
	}
}
