#include <pch.h>
#include <aiva2/deffered_renderer.hpp>

#include <aiva2/assert.hpp>
#include <aiva2/compute_material.hpp>
#include <aiva2/engine.hpp>
#include <aiva2/gpu_cmd_dispatch.hpp>
#include <aiva2/gpu_cmd_present.hpp>
#include <aiva2/gpu_cmd_set_render_target.hpp>
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
		init_random_effect();
	}

	deffered_renderer_t::~deffered_renderer_t()
	{
		shut_random_effect();
		shut_deffered_renderer();
	}

	void deffered_renderer_t::init_deffered_renderer()
	{
		get_engine().get_graphic_renderer().get_on_render_world().attach_listener(&this_type::tick_deffered_renderer, this);
	}

	void deffered_renderer_t::tick_deffered_renderer()
	{
		tick_random_effect();
		tick_present();
	}

	void deffered_renderer_t::shut_deffered_renderer()
	{
		get_engine().get_graphic_renderer().get_on_render_world().detach_listener(&this_type::tick_deffered_renderer, this);
	}

	void deffered_renderer_t::init_random_effect()
	{
		m_random_effect_material = get_engine().get_resource_system().get_resource<compute_material_t>("resources2/materials/random.json");
		assert_t::check_bool(m_random_effect_material, "m_random_effect is not valid");

		auto output_texture_info = tex_2d_info_t{};
		output_texture_info.set_format(buffer_format_t::R8G8B8A8_UNORM);
		output_texture_info.set_width(256);
		output_texture_info.set_height(256);
		output_texture_info.set_support_shader_resource(true);
		output_texture_info.set_support_unordered_access(true);

		auto output_texture = std::make_shared<tex_2d_t>(get_engine(), output_texture_info);
		assert_t::check_bool(output_texture, "output_texture is not valid");

		m_random_effect_output = output_texture;
	}

	void deffered_renderer_t::tick_random_effect()
	{
		assert_t::check_bool(m_random_effect_material, "m_random_effect_material is not valid");
		assert_t::check_bool(m_random_effect_output, "m_random_effect_output is not valid");
		
		auto const& main_texture_resource = m_random_effect_output;
		assert_t::check_bool(main_texture_resource, "main_texture_resource is not valid");

		auto const& main_texture_info = std::make_shared<uav_tex_2d_info_t>();
		assert_t::check_bool(main_texture_info, "main_texture_info is not valid");
		
		(*m_random_effect_material).set_resource("m_main_texture", main_texture_resource, main_texture_info);

		auto const dispatch_command = std::make_shared<gpu_cmd_dispatch_t>(get_engine());
		assert_t::check_bool(dispatch_command, "dispatch_command is not valid");

		(*dispatch_command).set_material_ptr(m_random_effect_material);
		(*dispatch_command).set_thread_group_count
		({
			(*m_random_effect_output).get_info().get_width() / 16,
			(*m_random_effect_output).get_info().get_height() / 16,
			1
		});

		get_engine().get_graphic_renderer().execute_render_command(dispatch_command);
	}

	void deffered_renderer_t::shut_random_effect()
	{
		assert_t::check_bool(m_random_effect_output, "m_random_effect_output is not valid");
		m_random_effect_output = {};

		assert_t::check_bool(m_random_effect_material, "m_random_effect_material is not valid");
		m_random_effect_material = {};
	}

	void deffered_renderer_t::tick_present()
	{
		auto const present_command = std::make_shared<gpu_cmd_present_t>(get_engine());
		assert_t::check_bool(present_command, "present_command is not valid");

		get_engine().get_graphic_renderer().execute_render_command(present_command);
	}
}
