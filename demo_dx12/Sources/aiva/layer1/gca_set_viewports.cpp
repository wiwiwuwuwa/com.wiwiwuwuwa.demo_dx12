#include <pch.h>
#include <aiva/layer1/gca_set_viewports.h>

#include <aiva/layer1/engine.h>
#include <aiva/layer1/graphic_hardware.h>

void aiva::layer1::GcaSetViewports::Execute(Engine const& engine) const
{
	auto const& commandList = engine.GraphicHardware().CommandList();
	winrt::check_bool(commandList);

	auto viewport = D3D12_VIEWPORT{};
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;
	viewport.Width = 100.0f;
	viewport.Height = 100.0f;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	commandList->RSSetViewports(1, &viewport);

	auto rect = D3D12_RECT{};
	rect.left = 0;
	rect.top = 0;
	rect.right = 100;
	rect.bottom = 100;

	commandList->RSSetScissorRects(1, &rect);
}
