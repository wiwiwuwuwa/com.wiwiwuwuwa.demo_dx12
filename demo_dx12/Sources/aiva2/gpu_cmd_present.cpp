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
		for (auto const& resource_barrier : get_engine().get_graphic_renderer().get_screen_target_ref().shut_for_rendering())
		{
			get_engine().get_graphic_hardware().get_command_list().ResourceBarrier(1, &resource_barrier);
		}
	}
}
