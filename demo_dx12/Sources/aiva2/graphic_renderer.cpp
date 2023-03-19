#include <pch.h>
#include <aiva2/graphic_renderer.hpp>

#include <aiva2/assert.hpp>
#include <aiva2/engine.hpp>
#include <aiva2/gpu_cmd_base.hpp>
#include <aiva2/graphic_executor.hpp>
#include <aiva2/graphic_hardware.hpp>
#include <aiva2/render_target.hpp>
#include <aiva2/rtv_tex_2d.hpp>
#include <aiva2/tex_2d.hpp>
#include <aiva2/time_system.hpp>

namespace aiva2
{
	graphic_renderer_t::graphic_renderer_t(engine_t& engine)
		: impl_type{ engine }
	{
		init_screen_targets();
		init_on_render_world();
	}

	graphic_renderer_t::~graphic_renderer_t()
	{
		shut_on_render_world();
		shut_screen_targets();
	}

	auto graphic_renderer_t::get_screen_target() const->std::shared_ptr<render_target_t> const&
	{
		auto const num_screen_targets = static_cast<size_t>(std::size(m_screen_targets));
		assert_t::check_bool(num_screen_targets > 0, "num_screen_targets is not valid");
		auto const screen_target_index = (get_engine().get_time_system().get_tick() + 1) % num_screen_targets;
		
		return m_screen_targets[screen_target_index];
	}

	void graphic_renderer_t::init_screen_targets()
	{
		m_screen_targets = {};

		auto swap_desc = DXGI_SWAP_CHAIN_DESC1{};
		assert_t::check_hresult(get_engine().get_graphic_hardware().get_swap_chain().GetDesc1(&swap_desc), "failed to get swap_desc");

		for (auto i = std::size_t{}; i < static_cast<size_t>(swap_desc.BufferCount); i++)
		{
			auto texture_resource = winrt::com_ptr<ID3D12Resource>{};
			assert_t::check_hresult(get_engine().get_graphic_hardware().get_swap_chain().GetBuffer(static_cast<UINT>(i), IID_PPV_ARGS(&texture_resource)), "failed to get texture_resource");
			assert_t::check_bool(texture_resource, "texture_resource is not valid");

			auto const& screen_target = m_screen_targets.emplace_back(std::make_shared<render_target_t>(get_engine()));
			assert_t::check_bool(screen_target, "screen_target is not valid");

			(*screen_target).add_color_texture<rtv_tex_2d_t>(texture_resource);
		}
	}

	void graphic_renderer_t::shut_screen_targets()
	{
		m_screen_targets = {};
	}
	
	auto graphic_renderer_t::get_on_render_world() -> event_action_type<>&
	{
		return m_on_render_world;
	}

	void graphic_renderer_t::init_on_render_world()
	{
		get_engine().get_time_system().get_on_pre_render().attach_listener(&this_type::tick_on_render_world, this);
	}

	void graphic_renderer_t::tick_on_render_world()
	{
		m_on_render_world.invoke();
	}
	
	void graphic_renderer_t::shut_on_render_world()
	{
		get_engine().get_time_system().get_on_pre_render().detach_listener(&this_type::tick_on_render_world, this);
	}

	void graphic_renderer_t::execute_render_command(std::shared_ptr<gpu_cmd_base_t> const& render_command)
	{
		get_engine().get_graphic_executor().execute_command(render_command);
	}
}
