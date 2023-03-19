#include <pch.h>
#include <aiva2/gpu_cmd_set_render_target.hpp>

#include <aiva2/assert.hpp>
#include <aiva2/engine.hpp>
#include <aiva2/graphic_hardware.hpp>
#include <aiva2/render_target.hpp>

namespace aiva2
{
	gpu_cmd_set_render_target_t::gpu_cmd_set_render_target_t(engine_t& engine)
		: impl_type{ engine }
	{
		
	}

	gpu_cmd_set_render_target_t::~gpu_cmd_set_render_target_t()
	{
		
	}

	void gpu_cmd_set_render_target_t::execute()
	{
		execute_resource_barrier();
		execute_set_render_target();
	}

	void gpu_cmd_set_render_target_t::execute_resource_barrier()
	{
		assert_t::check_bool(m_render_target, "m_render_target is not valid");
		(*m_render_target).init_for_rendering();
	}

	void gpu_cmd_set_render_target_t::execute_set_render_target()
	{
		assert_t::check_bool(m_render_target, "m_render_target is not valid");

		auto const color_texture_handle_opt = (*m_render_target).get_color_texture_handle();
		auto const color_texture_handle_num = (*m_render_target).num_color_texture_handle();
		auto const depth_texture_handle_opt = (*m_render_target).get_depth_texture_handle();
		
		get_engine().get_graphic_hardware().get_command_list().OMSetRenderTargets
		(
			/*NumRenderTargetDescriptors*/ static_cast<UINT>(color_texture_handle_num),
			/*pRenderTargetDescriptors*/ color_texture_handle_opt ? &(*color_texture_handle_opt) : nullptr,
			/*RTsSingleHandleToDescriptorRange*/ TRUE,
			/*pDepthStencilDescriptor*/ depth_texture_handle_opt ? &(*depth_texture_handle_opt) : nullptr
		);
	}

	auto gpu_cmd_set_render_target_t::get_render_target() const->std::shared_ptr<render_target_t> const&
	{
		return m_render_target;
	}

	void gpu_cmd_set_render_target_t::set_render_target(std::shared_ptr<render_target_t> const& render_target)
	{
		m_render_target = render_target;
	}
}
