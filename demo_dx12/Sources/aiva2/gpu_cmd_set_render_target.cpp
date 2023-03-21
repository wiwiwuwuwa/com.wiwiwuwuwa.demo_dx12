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
		execute_set_render_targets();
	}

	void gpu_cmd_set_render_target_t::execute_resource_barrier() const
	{
		if (!get_render_target_ptr())
		{
			return;
		}

		for (auto const& resource_barrier : get_render_target_ref().init_for_rendering())
		{
			get_engine().get_graphic_hardware().get_command_list().ResourceBarrier(1, &resource_barrier);
		}
	}

	void gpu_cmd_set_render_target_t::execute_set_render_targets() const
	{
		auto color_texture_handle_opt = std::optional<D3D12_CPU_DESCRIPTOR_HANDLE>{};
		auto color_texture_handle_num = size_t{};
		auto depth_texture_handle_opt = std::optional<D3D12_CPU_DESCRIPTOR_HANDLE>{};

		if (get_render_target_ptr())
		{
			color_texture_handle_opt = get_render_target_ref().get_color_texture_handle();
			color_texture_handle_num = get_render_target_ref().num_color_texture_handle();
			depth_texture_handle_opt = get_render_target_ref().get_depth_texture_handle();
		}

		get_engine().get_graphic_hardware().get_command_list().OMSetRenderTargets
		(
			/*NumRenderTargetDescriptors*/ static_cast<UINT>(color_texture_handle_num),
			/*pRenderTargetDescriptors*/ color_texture_handle_opt ? &(*color_texture_handle_opt) : nullptr,
			/*RTsSingleHandleToDescriptorRange*/ TRUE,
			/*pDepthStencilDescriptor*/ depth_texture_handle_opt ? &(*depth_texture_handle_opt) : nullptr
		);
	}

	auto gpu_cmd_set_render_target_t::get_render_target_ptr() const->std::shared_ptr<render_target_t> const&
	{
		return m_render_target;
	}
	
	auto gpu_cmd_set_render_target_t::get_render_target_ref() const->render_target_t&
	{
		assert_t::check_bool(m_render_target, "m_render_target is not valid");
		return (*m_render_target);
	}

	void gpu_cmd_set_render_target_t::set_render_target_ptr(std::shared_ptr<render_target_t> const& render_target)
	{
		m_render_target = render_target;
	}
}
