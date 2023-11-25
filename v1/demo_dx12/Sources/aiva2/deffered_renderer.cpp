#include <pch.h>
#include <aiva2/deffered_renderer.hpp>

#include <aiva2/assert.hpp>
#include <aiva2/compute_material.hpp>
#include <aiva2/engine.hpp>
#include <aiva2/gpu_cmd_clear_render_target.hpp>
#include <aiva2/gpu_cmd_dispatch.hpp>
#include <aiva2/gpu_cmd_draw_proc.hpp>
#include <aiva2/gpu_cmd_present.hpp>
#include <aiva2/gpu_cmd_set_render_target.hpp>
#include <aiva2/graphic_material.hpp>
#include <aiva2/graphic_renderer.hpp>
#include <aiva2/render_target.hpp>
#include <aiva2/resource_system.hpp>
#include <aiva2/tex_2d.hpp>
#include <aiva2/tex_2d_info.hpp>
#include <aiva2/uav_tex_2d_info.hpp>

namespace aiva2
{
	deffered_renderer_t::deffered_renderer_t(engine_t& engine)
		: impl_type{ engine }
	{
		init_deffered_renderer();
		init_triangle();
	}

	deffered_renderer_t::~deffered_renderer_t()
	{
		shut_triangle();
		shut_deffered_renderer();
	}

	void deffered_renderer_t::init_deffered_renderer()
	{
		get_engine().get_graphic_renderer().get_on_render_world().attach_listener(&this_type::tick_deffered_renderer, this);
	}

	void deffered_renderer_t::tick_deffered_renderer()
	{
		tick_triangle();
		tick_present();
	}

	void deffered_renderer_t::shut_deffered_renderer()
	{
		get_engine().get_graphic_renderer().get_on_render_world().detach_listener(&this_type::tick_deffered_renderer, this);
	}

	void deffered_renderer_t::init_triangle()
	{
		m_triangle_material = get_engine().get_resource_system().get_resource<graphic_material_t>("resources2/materials/triangle.json");
		assert_t::check_bool(m_triangle_material, "(m_triangle_material) is not valid");
	}

	void deffered_renderer_t::tick_triangle()
	{
		assert_t::check_bool(m_triangle_material, "(m_triangle_material) is not valid");

		auto const clear_render_target = std::make_shared<gpu_cmd_clear_render_target_t>(get_engine());
		assert_t::check_bool(clear_render_target, "(clear_render_target) is not valid");
		(*clear_render_target).set_render_target_ptr(get_engine().get_graphic_renderer().get_screen_target_ptr());
		(*clear_render_target).set_clear_color_opt(glm::vec4{ 0.0f, 0.0f, 0.0f, 1.0f });

		get_engine().get_graphic_renderer().execute_render_command(clear_render_target);

		auto const set_render_target = std::make_shared<gpu_cmd_set_render_target_t>(get_engine());
		assert_t::check_bool(set_render_target, "(set_render_target) is not valid");
		(*set_render_target).set_render_target_ptr(get_engine().get_graphic_renderer().get_screen_target_ptr());

		get_engine().get_graphic_renderer().execute_render_command(set_render_target);

		auto const draw_proc_command = std::make_shared<gpu_cmd_draw_proc_t>(get_engine());
		assert_t::check_bool(draw_proc_command, "(draw_proc_command) is not valid");
		(*draw_proc_command).set_material_ptr(m_triangle_material);
		(*draw_proc_command).set_vertices_count(3);
		(*draw_proc_command).set_instances_count(1);

		get_engine().get_graphic_renderer().execute_render_command(draw_proc_command);
	}

	void deffered_renderer_t::shut_triangle()
	{
		assert_t::check_bool(m_triangle_material, "(m_triangle_material) is not valid");
		m_triangle_material = {};
	}

	void deffered_renderer_t::tick_present()
	{
		auto const present_command = std::make_shared<gpu_cmd_present_t>(get_engine());
		assert_t::check_bool(present_command, "present_command is not valid");

		get_engine().get_graphic_renderer().execute_render_command(present_command);
	}
}
