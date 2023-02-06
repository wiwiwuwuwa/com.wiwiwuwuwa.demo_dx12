#include <pch.h>
#include <aiva2/native/uwp/gce_set_viewports.hpp>

#include <aiva2/native/engine.hpp>
#include <aiva2/native/gcd_set_viewports.hpp>
#include <aiva2/native/graphic_hardware.hpp>

namespace aiva2::native
{	
	void gce_set_viewports_t::execute(gcd_set_viewports_t& command, engine_t& engine)
	{
		execute_set_scissor_rects(command, engine);
		execute_set_viewports(command, engine);
	}

	void gce_set_viewports_t::execute_set_scissor_rects(gcd_set_viewports_t& command, engine_t& engine)
	{
		auto scissor_rect = D3D12_RECT{};
		scissor_rect.left = static_cast<LONG>(command.get_rect().x);
		scissor_rect.top = static_cast<LONG>(command.get_rect().y);
		scissor_rect.right = static_cast<LONG>(command.get_rect().z);
		scissor_rect.bottom = static_cast<LONG>(command.get_rect().w);

		engine.get_graphic_hardware().get_command_list().RSSetScissorRects(1, &scissor_rect);
	}

	void gce_set_viewports_t::execute_set_viewports(gcd_set_viewports_t& command, engine_t& engine)
	{
		auto viewport = D3D12_VIEWPORT{};
		viewport.TopLeftX = command.get_rect().x;
		viewport.TopLeftY = command.get_rect().y;
		viewport.Width = command.get_rect().z - command.get_rect().x;
		viewport.Height = command.get_rect().w - command.get_rect().y;
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;

		engine.get_graphic_hardware().get_command_list().RSSetViewports(1, &viewport);
	}
}
