#include <pch.h>
#include <aiva2/deffered_renderer.hpp>

#include <aiva2/engine.hpp>
#include <aiva2/gpu_cmd_present.hpp>
#include <aiva2/gpu_cmd_set_render_target.hpp>
#include <aiva2/graphic_renderer.hpp>
#include <aiva2/render_target.hpp>

namespace aiva2
{
	deffered_renderer_t::deffered_renderer_t(engine_t& engine)
		: impl_type{ engine }
	{
		init_deffered_renderer();
	}

	deffered_renderer_t::~deffered_renderer_t()
	{
		shut_deffered_renderer();
	}

	void deffered_renderer_t::init_deffered_renderer()
	{
		get_engine().get_graphic_renderer().get_on_render_world().attach_listener(&this_type::tick_deffered_renderer, this);
	}

	void deffered_renderer_t::tick_deffered_renderer()
	{
		auto const set_render_target_command = std::make_shared<gpu_cmd_set_render_target_t>(get_engine());
		(*set_render_target_command).set_render_target_ptr(get_engine().get_graphic_renderer().get_screen_target_ptr());
		get_engine().get_graphic_renderer().execute_render_command(set_render_target_command);

		auto const present_command = std::make_shared<gpu_cmd_present_t>(get_engine());
		get_engine().get_graphic_renderer().execute_render_command(present_command);
	}

	void deffered_renderer_t::shut_deffered_renderer()
	{
		get_engine().get_graphic_renderer().get_on_render_world().detach_listener(&this_type::tick_deffered_renderer, this);
	}
}
