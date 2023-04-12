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
		execute_rs_set_scissor_rects();
		execute_rs_set_viewports();
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
		auto color_handle_opt = std::optional<D3D12_CPU_DESCRIPTOR_HANDLE>{};
		auto color_handle_num = size_t{};
		auto depth_handle_opt = std::optional<D3D12_CPU_DESCRIPTOR_HANDLE>{};

		if (get_render_target_ptr())
		{
			color_handle_opt = get_render_target_ref().get_color_handle();
			color_handle_num = get_render_target_ref().num_color_handle();
			depth_handle_opt = get_render_target_ref().get_depth_handle();
		}

		get_engine().get_graphic_hardware().get_command_list().OMSetRenderTargets
		(
			/*NumRenderTargetDescriptors*/ static_cast<UINT>(color_handle_num),
			/*pRenderTargetDescriptors*/ color_handle_opt ? &(*color_handle_opt) : nullptr,
			/*RTsSingleHandleToDescriptorRange*/ TRUE,
			/*pDepthStencilDescriptor*/ depth_handle_opt ? &(*depth_handle_opt) : nullptr
		);
	}

	void gpu_cmd_set_render_target_t::execute_rs_set_scissor_rects() const
	{
		auto scissor_rects = std::vector<D3D12_RECT>{};

		if (auto const render_target = get_render_target_ptr())
		{
			for (auto i = size_t{}; i < (*render_target).num_viewport_size(); i++)
			{
				auto const size = (*render_target).get_viewport_size(i);
				assert_t::check_bool(size.x > decltype(size.x){}, "(size.x) is not valid");
				assert_t::check_bool(size.y > decltype(size.y){}, "(size.y) is not valid");

				auto& scissor_rect = scissor_rects.emplace_back();
				scissor_rect.left = 0;
				scissor_rect.top = 0;
				scissor_rect.right = static_cast<LONG>(size.x);
				scissor_rect.bottom = static_cast<LONG>(size.y);
			}
		}

		get_engine().get_graphic_hardware().get_command_list().RSSetScissorRects
		(
			/*NumRects*/ static_cast<UINT>(std::size(scissor_rects)),
			/*pRects*/ std::data(scissor_rects)
		);
	}

	void gpu_cmd_set_render_target_t::execute_rs_set_viewports() const
	{
		auto viewports = std::vector<D3D12_VIEWPORT>{};

		if (auto const render_target = get_render_target_ptr())
		{
			for (auto i = size_t{}; i < (*render_target).num_viewport_size(); i++)
			{
				auto const size = (*render_target).get_viewport_size(i);
				assert_t::check_bool(size.x > decltype(size.x){}, "(size.x) is not valid");
				assert_t::check_bool(size.y > decltype(size.y){}, "(size.y) is not valid");

				auto& viewport = viewports.emplace_back();
				viewport.TopLeftX = 0.0f;
				viewport.TopLeftY = 0.0f;
				viewport.Width = static_cast<FLOAT>(size.x);
				viewport.Height = static_cast<FLOAT>(size.y);
				viewport.MinDepth = 0.0f;
				viewport.MaxDepth = 1.0f;
			}
		}

		get_engine().get_graphic_hardware().get_command_list().RSSetViewports
		(
			/*NumViewports*/ static_cast<UINT>(std::size(viewports)),
			/*pViewports*/ std::data(viewports)
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
